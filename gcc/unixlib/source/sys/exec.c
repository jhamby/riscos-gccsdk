/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/exec,v $
 * $Date: 2000/08/17 16:16:06 $
 * $Revision: 1.32 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: exec,v 1.32 2000/08/17 16:16:06 admin Exp $";
#endif

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/unix.h>
#include <sys/syslib.h>
#include <sys/swis.h>
#include <sys/wait.h>
#include <unixlib/local.h>

/* #define DEBUG 1 */

#ifdef DEBUG
#include <sys/debug.h>
#include <sys/os.h>
#include <stdio.h>
#endif


#ifdef DEBUG
#define ushift(p,v,c) ((p) = __ushift((unsigned char *)p,v,c,"(" #p ")"))
/* type cast version of ushift.  */
#define ushift2(p,v,c,t) ((p) = (t)__ushift((unsigned char *)p,v,c,"(" #p ")"))
#define showmove(s,n,p,r) \
  (os_print(s "("), os_print(n), os_prhex((int)p), os_print(") -> "), \
   os_prhex((int)r), os_nl())
#undef memcpy
#define memcpy(d,s,n) \
  (os_print("memcpy("), os_prhex((int)(d)), os_print(","), os_prhex((int)(s)), \
   os_print(","), os_prhex((int)(n)), os_print(")"), os_nl(), (memcpy)(d,s,n))

/* Relocate a pointer.
   `p' - pointer to relocate.
   `v' - variable
   `c' - code
   `n' - comment string.

   If `p' is located between __base and __rwlimit (i.e. the code section
   of the program) then it will be relocated using `c'.

   If `p' is located between __lomem and __break (i.e. the data section
   of the program) then it will be relocated using `v'.  */
static void *
__ushift (unsigned char *p, unsigned int v, unsigned int c, char *n)
{
  void *r = (((void *)p >= __base && (void *)p < __rwlimit) ? p + c
	     : ((void *)p >= __lomem && (void *)p < __break) ? p + v : p);
  showmove ("ushift", n, p, r);
  return r;
}

#else

#define ushift(p,v,c) ((p) = __ushift((unsigned char *)p,v,c))
#define ushift2(p,v,c,t) ((p) = (t)__ushift((unsigned char *)p,v,c))

static void *
__ushift (unsigned char *p, unsigned int v, unsigned int c)
{
  return (((void *)p >= __base && (void *)p < __rwlimit) ? p + c
	  : ((void *)p >= __lomem && (void *)p < __break) ? p + v : p);
}

#endif

#ifdef DEBUG
#define dshift(p,v,c) ((p) = __dshift((unsigned char *)p,v,c,"(" #p ")"))
#define dshift2(p,v,c,t) ((p) = (t)__dshift((unsigned char *)p,v,c,"(" #p ")"))

static void *
__dshift (unsigned char *p, unsigned int v, unsigned int c, char *n)
{
  void *q1 = p - c;
  void *q2 = p - v;
  void *r = ((q1 >= __base && q1 < __rwlimit) ? q1
	     : (q2 >= __lomem && q2 < __break) ? q2 : p);
  showmove ("dshift", n, p, r);
  return r;
}

#else

#define dshift(p,v,c) ((p) = __dshift((unsigned char *)p,v,c))
#define dshift2(p,v,c,t) ((p) = (t)__dshift((unsigned char *)p,v,c))

static void *
__dshift (unsigned char *p, unsigned int v, unsigned int c)
{
  void *q1 = p - c;
  void *q2 = p - v;

  /* This is only safe when 2 * maximum application space size < start of
     dynamic areas. This is not true, see PJB any real restrictions.  */
  return ((q1 >= __base && q1 < __rwlimit) ? q1
	  : (q2 >= __lomem && q2 < __break) ? q2 : p);
}

#endif

/* Setup the extended command line by sending all the arguments to the
   program via dde utils.  */
static int
set_dde_cli (char *cli)
{
  int regs[10];
  char *temp = cli;

  if (*temp == '*')		/* skip any leading star and whitespace.  */
    temp++;
  while (*temp == ' ')
    temp++;
  temp = strchr (temp, ' ');	/* skip program name.  */

  /* Check that the actual command is not greater than
     RISC OS's maximum path length.  */
  if (!temp || (temp - cli) >= MAXPATHLEN)
    return __set_errno (E2BIG);

  /* Set the command line size within DDE utils.  */
  regs[0] = strlen (temp + 1) + 1;
  if (os_swi (DDEUtils_SetCLSize, regs))
    /* We're buggered if DDE utils isn't on this system.  */
    return __set_errno (E2BIG);

  regs[0] = (int) temp + 1;
  os_swi (DDEUtils_SetCL, regs);
  *temp = '\0';		/* terminate cli.  */
#ifdef DEBUG
  os_print ("DDE utils set up\n\r");
#endif
  return 0;
}

_kernel_oserror *__exerr;

/* Execute program `execname' and pass command line arguments `argv'
   and give it the environment `envp'.  */
int
execve (const char *execname, char **argv, char **envp)
{
  struct proc *process = __u;
  int x, cli_length;
  const char *program_name;
  char *command_line;
  void (*__exec) (char *);
  char *cli;
  char pathname[MAXPATHLEN];	/* There is scope to merge this buffer with
				   cli. I don't feel brave enough */

  if (! execname || ! argv || ! envp)
    return __set_errno (EINVAL);

#ifdef DEBUG
  os_print ("-- execve: function arguments\r\n");
  os_print ("      execname: "); os_print (execname); os_print ("\r\n");
  x = -1;
  while (argv[++x])
    {
      os_print ("      argv["); os_prdec (x); os_print ("]: ");
      os_print (argv[x]); os_print ("\r\n");
    }
  x = -1;
  while (envp[++x])
    {
      os_print ("      envp["); os_prdec (x); os_print ("]: ");
      os_print (envp[x]); os_print ("\r\n");
    }

  __debug ("-- execve: process structure");
#endif

  if (*execname != '*')
    {
      /* Convert the program name into a RISC OS format filename.  */
      program_name = __riscosify_std (execname, 0, pathname,
				      sizeof (pathname), NULL);
      if (program_name == NULL)
	return __set_errno (E2BIG);

      cli_length = program_name - pathname + 1;	/* strlen + 1 */
      program_name = pathname;		/* This is copied into cli + 1 */
    }
  else
    {
      program_name = execname;
      cli_length = strlen (program_name) + 1;
    }

#if __FEATURE_ITIMERS
  /* Stop any interval timers that might be running.  Technically
     the new process should inherit the pending alarms.  */
  __stop_itimers ();
#endif

#ifdef DEBUG
  os_print ("-- execve: building command line\r\n");
#endif

  /* Calculate the length of the command line.  We need to do this
     because the program that we are about to run might not
     be a UnixLib compatable binary so it won't know anything about
     the child process structure.  */
  cli_length = 0;
  for (x = 0; argv[x]; x++)
    {
      char *p;
      int space = 0;
      /* We must add extra characters if the argument contains spaces (wrap
         argument in quotes) and quotes (must add backslash) and
         inverted commas (must add backslash).  */
      for (p = argv[x]; *p; p++)
        {
          if (isspace (*p))
            space = 1;
          if (*p == '\"' || *p == '\'')
            cli_length ++;
          cli_length ++;
        }
      if (space)
        cli_length += 2; /* Account for quotes around argument.  */
      cli_length ++;
    }

#ifdef DEBUG
  os_print ("-- execve: cli_length = "); os_prdec (cli_length);
  os_print ("\r\n");
#endif

  /* Hmmm.  This malloc *must not* be in a dynamic area.  */
  cli = (char *) malloc (cli_length + 1);
  if (cli == NULL)
    return __set_errno (ENOMEM);

#ifdef DEBUG
  os_print ("-- execve: here 2\r\n");
#endif

  /* Copy program name into cli and terminate with a space, ready for below.
     Should trim leading space between '*' and command in name.  */
  command_line = cli;
  while (*program_name == '*' || *program_name == ' ')
    program_name ++;
  command_line = stpcpy (command_line, program_name);
  *command_line ++ = ' ';

#ifdef DEBUG
  os_print ("-- execve: here 3\r\n");
#endif
  /* Copy the rest of the arguments into the cli.  */
  if (argv[0])
    for (x = 1; argv[x]; x++)
      {
        char *p = argv[x];
        int contains_space = 0;
        while (*p != '\0' && ! isspace (*p))
          p++;
        if (*p)
          contains_space = 1;

        /* Add quotes, if argument contains a space.  */
	if (contains_space)
	  *command_line ++ = '\"';
        for (p = argv[x]; *p; p++)
          {
            /* If character is a " or a ' then preceed with a backslash.  */
            if (*p == '\"' || *p == '\'')
              *command_line ++ = '\\';
            *command_line ++ = *p;
          }
        if (contains_space)
          *command_line ++ = '\"';
        *command_line ++ = ' ';
      }
  command_line[-1] = '\0';

#ifdef DEBUG
  os_print ("-- execve: cli: "); os_print (cli); os_print ("\r\n");
#endif

  /* This `if' will never return.  */
  if (! process->status.has_parent)
    {
#ifdef DEBUG
      os_print ("-- execve: take a shortcut (not a child process)\r\n");
#endif
      /* Shutdown unix. This is literally the point of no return.  */
      __reset ();

      /* If the cli is >= MAXPATHLEN, we will need the aid of DDE utils.  */
      if (strlen (cli) >= MAXPATHLEN && set_dde_cli (cli) < 0)
	__exit_no_code ();	/* No return possible (see above).  */

      /* If the DAs are being used, then delete the dynamic area first.
	 This is safe to do before the os_cli call since cli is on the stack
	 and hence not in the DA.  Firstly, trim the area back with brk().  */
      if (__dynamic_num != -1)
	{
	  brk (__lomem);
	  __dynamic_area_exit ();
	}

      /* Pass the command to os_cli (which never returns).  */
      os_cli (cli);

      /* If we've called a RISC OS builtin then we can return here.
         If we haven't then something ugly must have happened.  There
         isn't much be can do, so just exit.  */
      {
	 int regs[10];

	 regs[0] = 0;
	 os_swi (DDEUtils_SetCLSize, regs);
      }
      __exit_no_code ();
      /* NOTREACHED */
    }

  /* If we arrive here, we have a parent process.  */
#ifdef DEBUG
  os_print ("-- execve: copy argv and envp (we are a child proc)\r\n");
#endif
  /* We enter here if exec was called after a vfork, which is
     usually the case.  */

  /* Free the memory we previously allocated to the child process's
     argument vectors.  The child process will create this itself.
     However, we do create the process's environment vectors.
     From now on, this is the point of no return.  */
#ifdef DEBUG
  os_print ("-- execve: proc->envp="); os_prhex ((int) process->envp); os_nl ();
  os_print ("-- execve: proc->argv="); os_prhex ((int) process->argv); os_nl ();
#endif

  if (process->envp)
    {
      for (x = 0; x < process->envc; x++)
        if (process->envp[x])
          free (process->envp[x]);
      free (process->envp);
    }
  if (process->argv)
    {
      for (x = 0; x < process->argc; x++)
        if (process->argv[x])
          free (process->argv[x]);
      free (process->argv);
    }

  /* Count new environment variable vector length.  */
  for (x = 0; envp[x]; x++)
    ;
  process->envc = x;
  if (process->envc)
    {
      /* Make a copy of the command line arguments.  */
      process->envp = (char **) malloc ((process->envc + 2)
      	       	       	    	       	* sizeof (char *));
      if (process->envp == NULL)
        __exit_no_code ();
      for (x = 0; x < process->envc; x++)
        {
          process->envp[x] = strdup (envp[x]);
          if (process->envp[x] == NULL)
            __exit_no_code ();
        }
      /* Terminate the environment list.  */
      process->envp[process->envc] = NULL;
    }
  else
    process->envp = NULL;

  process->argc = 0;
  process->argv = NULL;

  /* If the cli is >= MAXPATHLEN, we will need the aid of DDE utils.  */
  if (strlen (cli) >= MAXPATHLEN && set_dde_cli (cli) < 0)
    return -1;

  /* File descriptors open in the existing process image remain open
     in the new process image, unless they have the 'FD_CLOEXEC'
     flag set.  O_EXECCL is an alias for FD_CLOEXEC.  */
  for (x = 0; x < MAXFD; x++)
    if (process->fd[x].dflag & O_EXECCL)
      close (x);

  process->status.has_parent = 0;

#ifdef DEBUG
  __debug ("-- execve: process after new argv and envp setup");
  os_print ("__rwlimit: "); os_prhex ((unsigned int) __rwlimit); os_nl ();
  os_print ("__stack_limit: "); os_prhex ((unsigned int) __stack_limit);
  os_nl ();
#endif

  /* Force a malloc trim to reduce memory usage.  malloc() must not be
     called after the __codeshift calculation below since __stack_limit
     could be increased by any call to malloc().  Thus, malloc trim here
     is guaranteed to minimise the malloc heap as best as possible
     before running the child program.  */
  malloc_trim (0);

  if (((unsigned int) __base & ~0xff) == 0x8000)
    {
      __codeshift = ((char *) __stack - (char *) __stack_limit) - 512 - __exlen;
#ifdef __4K_BOUNDARY
      /* Align down to 4K boundary.  */
      __codeshift = __codeshift & ~4095;
#endif
      /* Heap might not be in a dynamic area (dynamic_num == -1).  */
      __exshift = (__dynamic_num == -1) ? __codeshift : 0;
    }
  else
    __codeshift = __exshift = 0;

  /* __codeshift needs to be > 0, but how much is a suitable minimum ?  */
  if (__codeshift < cli_length)
    return __set_errno (ENOMEM);

#ifdef DEBUG
  os_print ("__exlen: "); os_prhex ((unsigned int) __exlen);
  os_print ("__codeshift: "); os_prhex ((unsigned int) __codeshift);
  os_print ("__exshift: "); os_prhex ((unsigned int) __exshift);
  os_nl ();
#endif

  /* Remove environment handlers.  */
  __restore_calling_environment_handlers ();

#ifdef DEBUG
  os_print ("-- execve: environment handlers restored\r\n");
#endif

  /* Shift __u pointers if necessary.  */
  if (__exshift || __codeshift)
    {
      unsigned int variable = __exshift;
      unsigned int code = __codeshift;
      int i;

      /* Pointers located between __base and __rwlimit (i.e. the code
	 section of a program) will be relocated using 'code'.

	 Pointers located between __lomem and __break (i.e. the data
	 section of a program) will be relocated using 'variable'.  */
      for (i = 0; i <= process->envc; i++)
	ushift (process->envp[i], variable, code);
      if (process->envp)
        ushift (process->envp, variable, code);
      for (i = 0; i < MAXTTY; i++)
	{
	  if (process->tty[i].out)
	    ushift2 (process->tty[i].out, variable, code, int (*)(int));
	  if (process->tty[i].in)
	    ushift2 (process->tty[i].in, variable, code, int (*)(void));
	  if (process->tty[i].scan)
	    ushift2 (process->tty[i].scan, variable, code, int (*)(int));
	  if (process->tty[i].init)
	    ushift2 (process->tty[i].init, variable, code, int (*)(void));
	  if (process->tty[i].flush)
	    ushift2 (process->tty[i].flush, variable, code, int (*)(void));

#ifndef __TTY_STATIC_BUFS
	  if (process->tty[i].del)
	    ushift (process->tty[i].del, variable, code);
	  if (process->tty[i].buf)
	    ushift (process->tty[i].buf, variable, code);
#endif
	  if (process->tty[i].ptr)
	    ushift (process->tty[i].ptr, variable, code);
	}
      ushift (process->tty, variable, code);

      __exec = (void (*)(char *)) ((char *) __stack_limit + __codeshift);
    }
  else
    __exec = __exptr;

  /* Set address of proc struct for child. This WILL change soon.  */
  {
    int regs[10];
    char *address;

    address = (char *) process + __exshift;
    regs[0] = (int) "UnixLib$env";
    regs[1] = (int) &address;
    regs[2] = 4;
    regs[3] = 0;
    regs[4] = 1;
    os_swi (OS_SetVarVal, regs);
  }

  /* copy up m/code routine and heap */

  if (__codeshift)
    {
      int regs[10];

      memcpy ((char *) __exec, (char *) __exptr, __exlen);
      /* SynchroniseCodeAreas. This call is okay even if it isn't necessary
	 on pre-StrongARM machines.  */
      regs[0] = 1;
      regs[1] = (int) __exec;
      regs[2] = regs[1] + __exlen - 4;
      os_swi (OS_SynchroniseCodeAreas, regs);
    }
  if (__exshift)
    memcpy ((char *) __lomem + __exshift, (char *) __lomem,
	    (char *) __break - (char *) __lomem);

  /* Finally call the program.  */
#ifdef DEBUG
  os_print ("-- execve: about to call:"); os_print (cli); os_nl ();
#endif
  __funcall ((*__exec), (cli));
  /* This is never reached.  */
  return 0;
}

void
__exret (void)
{
  struct env *environment;
  int i;

  /* Shift heap and __u pointers back down.  */
  if (__exshift || __codeshift)
    {
      unsigned int variable = __exshift;
      unsigned int code = __codeshift;
      struct proc *process = __u;

      if (__exshift)
	memcpy ((char *) __lomem,
		(char *) __lomem + __exshift,
		(char *) __break - (char *) __lomem);
      dshift (process->tty, variable, code);
      for (i = 0; i < MAXTTY; i++)
	{
	  if (process->tty[i].out)
	    dshift2 (process->tty[i].out, variable, code, int (*)(int));
	  if (process->tty[i].in)
	    dshift2 (process->tty[i].in, variable, code, int (*)(void));
	  if (process->tty[i].scan)
	    dshift2 (process->tty[i].scan, variable, code, int (*)(int));
	  if (process->tty[i].init)
	    dshift2 (process->tty[i].init, variable, code, int (*)(void));
	  if (process->tty[i].flush)
	    dshift2 (process->tty[i].flush, variable, code, int (*)(void));

#ifndef __TTY_STATIC_BUFS
	  if (process->tty[i].del)
	    dshift (process->tty[i].del, variable, code);
	  if (process->tty[i].buf)
	    dshift (process->tty[i].buf, variable, code);
#endif
	  if (process->tty[i].ptr)
	    dshift (process->tty[i].ptr, variable, code);
	}
      if (process->envp)
        dshift (process->envp, variable, code);
      for (i = 0; i <= process->envc; i++)
	dshift (process->envp[i], variable, code);
      process->argv = NULL;
    }

  /* Reinstall environment handlers.  */
  environment = __c_environment;
  for (i = 0; i < __ENVIRONMENT_HANDLERS; i++)
    __write_environment_handler (i, environment->handlers + i);
  __remenv_from_os ("UnixLib$env");

#ifdef DEBUG
  __debug ("-- __exret: process table has just been relocated");
#endif

  __u->status.has_parent = 1;
  if (__exerr)
    {
      __seterr (__exerr);
      /* Encode the signal. See sys.c.vfork.  */
      i = (SIGERR) | (1 << 7);
    }
  else
    i = __intenv ("Sys$ReturnCode");

#ifdef DEBUG
  os_print ("-- __exret: return code="); os_prdec (i); os_nl ();
#endif

  if (___vret)
    __funcall ((*___vret), (i));
  else				/* oh fuck... */
    _exit (1);
}
