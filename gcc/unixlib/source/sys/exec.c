/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/exec.c,v $
 * $Date: 2003/01/05 12:27:55 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: exec.c,v 1.7 2003/01/05 12:27:55 admin Exp $";
#endif

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/param.h>
#include <unixlib/unix.h>
#include <unixlib/os.h>
#include <swis.h>
#include <sys/wait.h>
#include <unixlib/local.h>
#include <unixlib/features.h>
#include <pthread.h>

/* #define DEBUG 1 */

#ifdef DEBUG
#include <sys/debug.h>
#include <unixlib/os.h>
#include <stdio.h>
#endif


#ifdef DEBUG
#define ushift(p,v,c) ((p) = __ushift((unsigned char *)p,v,c,"(" #p ")"))
/* type cast version of ushift.  */
#define ushift2(p,v,c,t) ((p) = (t)__ushift((unsigned char *)p,v,c,"(" #p ")"))
#define showmove(s,n,p,r) \
  (__os_print(s "("), __os_print(n), __os_prhex((int)p), __os_print(") -> "), \
   __os_prhex((int)r), __os_nl())
#undef memcpy
#define memcpy(d,s,n) \
  (__os_print("memcpy("), __os_prhex((int)(d)), __os_print(","), __os_prhex((int)(s)), \
   __os_print(","), __os_prhex((int)(n)), __os_print(")"), __os_nl(), (memcpy)(d,s,n))

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
	     : ((void *)p >= __lomem && (void *)p < __stack_limit) ? p + v : p);
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
	  : ((void *)p >= __lomem && (void *)p < __stack_limit) ? p + v : p);
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
	     : (q2 >= __lomem && q2 < __stack_limit) ? q2 : p);
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
	  : (q2 >= __lomem && q2 < __stack_limit) ? q2 : p);
}

#endif

/* Setup the extended command line by sending all the arguments to the
   program via dde utils.  */
static int
set_dde_cli (char *cli)
{
  int regs[10];
  char *temp;

  /* Skip program name.  */
  temp = cli;
  while (*temp && *temp != ' ')
    temp ++;

  /* Check that the actual command is not greater than
     RISC OS's maximum path length.  */
  if ((temp - cli) >= MAXPATHLEN)
    return __set_errno (E2BIG);

  /* Set the command line size within DDE utils.  */
  regs[0] = strlen (temp + 1) + 1;
  if (__os_swi (DDEUtils_SetCLSize, regs))
    /* We're buggered if DDE utils isn't on this system.  */
    return __set_errno (E2BIG);

  regs[0] = (int) temp + 1;
  __os_swi (DDEUtils_SetCL, regs);

  *temp = '\0';		/* terminate cli.  */
#ifdef DEBUG
  __os_print ("DDE utils set up\n\r");
#endif
  return 0;
}

/* Also referenced in sys/_exec.s.  */
_kernel_oserror *__exerr;

/* Execute program `execname' and pass command line arguments `argv'
   and give it the environment `envp'.  */
int
execve (const char *execname, char *const argv[], char *const envp[])
{
  struct proc *process = __u;
  int x, cli_length;
  const char *program_name;
  char *command_line;
  void (*__exec) (char *);
  char *cli;
  char pathname[MAXPATHLEN];
  int nasty_hack = 0;
  char *null_list = NULL;

  if (! execname)
    return __set_errno (EINVAL);

  if (! argv)
    argv = &null_list;

  if (! envp)
    envp = &null_list;

#ifdef DEBUG
  __os_print ("-- execve: function arguments\r\n");
  __os_print ("      execname: '"); __os_print (execname); __os_print ("'\r\n");
  x = -1;
  while (argv[++x])
    {
      __os_print ("      argv["); __os_prdec (x); __os_print ("]: ");
      __os_print (argv[x]); __os_print ("\r\n");
    }
  x = -1;
  while (envp[++x])
    {
      __os_print ("      envp["); __os_prdec (x); __os_print ("]: ");
      __os_print (envp[x]); __os_print ("\r\n");
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

      program_name = pathname;		/* This is copied into cli + 1 */
    }
  else
    {
      /* Watch out ! If we've arrived here from a user call to system(),
	 the we could find that execname == "*" because on RISC OS
	 the '*' is equivalent to the SHELL of /bin/bash on Unix.
	 If this is the case, the we'll probably find the real program
	 name in argv[1].  */
      if (execname[1] == '\0')
	{
	  char *p = argv[1];
	  char temp[MAXPATHLEN];
	  x = 0;
	  while (x < MAXPATHLEN && *p && *p != ' ')
	    temp[x++] = *p++;

	  if (x == MAXPATHLEN)
	    return __set_errno (E2BIG);
	  temp[x] = '\0';

	  /* Ah. The nasty hack.  Comes into everything somewhere.
	     Since argv[1] contains our program name and all arguments,
	     we need to stop the command line builder sticking quotes
	     around everything and also not include the program name
	     twice in the argument vector.

	     So nasty_hack contains the length of the program name
	     held within argv[1].  We then know (when non-zero) how
	     many characters to skip, if at all.  */
	  nasty_hack = x;

#ifdef DEBUG
	  __os_print ("-- execve: pathname: '");
	  __os_print (temp); __os_print ("'\r\n");
#endif

	  program_name = __riscosify_std (temp, 0, pathname,
					  sizeof (pathname), NULL);
	  if (program_name == NULL)
	    return __set_errno (E2BIG);
	  program_name = pathname;
	}
      else
	{
	  program_name = execname;
	}
    }

#ifdef DEBUG
  __os_print ("-- execve: program_name: "); __os_print (program_name);
  __os_print ("\r\n");
#endif

#if __FEATURE_PTHREADS
  /* All threads are terminated on an exec call.
     Destructor functions are not called. */
  if (__pthread_system_running)
    {
      __pthread_stop_ticker ();
      __pthread_system_running = 0;
    }
#endif

#if __FEATURE_ITIMERS
  /* Stop any interval timers that might be running.  Technically
     the new process should inherit the pending alarms.  */
  __stop_itimers ();
#endif

#ifdef DEBUG
  __os_print ("-- execve: building command line\r\n");
#endif

  /* Calculate the length of the command line.  We need to do this
     because the program that we are about to run might not
     be a UnixLib compatable binary so it won't know anything about
     the child process structure.  */
  cli_length = strlen (program_name) + 1;
  x = (nasty_hack) ? 1 : 0;
  for (; argv[x]; x++)
    {
      char *p;
      int space = 0;
      /* We must add extra characters if the argument contains spaces (wrap
         argument in quotes) and quotes (must add backslash) and
         inverted commas (must add backslash) and control chars
         (change to \xXX).  */
      p = argv[x];
      if (nasty_hack && x == 1)
	p += nasty_hack;
      while (*p)
        {
          if (isspace (*p))
            space = 1;
          if ((!nasty_hack || x != 1) && (*p == '\"' || *p == '\''))
            cli_length ++;
          if (*p == 127 || *p < 32)
            cli_length +=3;
          cli_length ++;
	  p++;
        }

      /* Account for quotes around argument but only if our hack isn't
         in place.  */
      if (space && ! nasty_hack)
        cli_length += 2;

      cli_length ++;
    }

#ifdef DEBUG
  __os_print ("-- execve: cli_length = "); __os_prdec (cli_length);
  __os_print ("\r\n");
#endif

  /* This malloc *must not* be in a dynamic area.  */
  cli = (char *) __stackalloc (cli_length + 1);
  __u->cli = cli;
  if (cli == NULL)
    return __set_errno (ENOMEM);

  /* Copy program name into cli and terminate with a space, ready for below.
     Should trim leading space between '*' and command in name.  */
  command_line = cli;
  while (*program_name == '*' || *program_name == ' ')
    program_name ++;

  command_line = stpcpy (command_line, program_name);
  *command_line ++ = ' ';

  /* Copy the rest of the arguments into the cli.  */
  if (argv[0])
    for (x = 1; argv[x]; x++)
      {
        char *p = argv[x];
        int contains_space = 0;

	if (nasty_hack && x == 1)
	  p += nasty_hack;

        while (*p != '\0' && ! isspace (*p))
          p++;

	/* Don't enclose arguments in additional quotes if our nasty hack
	   is in place.  */
        if (! nasty_hack && *p)
          contains_space = 1;

        /* Add quotes, if argument contains a space.  */
	if (contains_space)
	  *command_line ++ = '\"';

	p = argv[x];
	if (nasty_hack && x == 1)
	  p += nasty_hack;
	while (*p)
	  {
            /* If character is a " or a ' then preceed with a backslash.  */
            if ((!nasty_hack || x != 1) && (*p == '\"' || *p == '\''))
              *command_line ++ = '\\';

            if (*p == 127 || *p < 32)
              {
                sprintf (command_line, "\\x%.2X", *p);
                command_line += 4;
              }
            else
              *command_line ++ = *p;

	    p++;
	  }
        if (contains_space)
          *command_line ++ = '\"';
        *command_line ++ = ' ';
      }
  command_line[-1] = '\0';

#ifdef DEBUG
  __os_print ("-- execve: cli: "); __os_print (cli); __os_print ("\r\n");
#endif

  /* This `if' will never return.  */
  if (! process->status.has_parent)
    {
#ifdef DEBUG
      __os_print ("-- execve: take a shortcut (not a child process)\r\n");
#endif
      /* Shutdown unix. This is literally the point of no return.  */
      __stop_itimers ();
      for (x = 0; x < MAXFD; x++)
	if (process->fd[x].__magic == _FDMAGIC)
	  close (x);

      /* Restore the RISC OS environment handlers.  This breaks us out
	 of the UnixLib world.  */
      __env_riscos ();

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
      __os_cli (cli);

      /* If we've called a RISC OS builtin then we can return here.
         If we haven't then something ugly must have happened.  There
         isn't much be can do, so just exit.  */
      {
	 int regs[10];

	 regs[0] = 0;
	 __os_swi (DDEUtils_SetCLSize, regs);
      }
      __exit_no_code ();
      /* NOTREACHED */
    }

  /* If we arrive here, we have a parent process.  */
#ifdef DEBUG
  __os_print ("-- execve: copy argv and envp (we are a child proc)\r\n");
#endif
  /* We enter here if exec was called after a vfork, which is
     usually the case.  */

  /* Free the memory we previously allocated to the child process's
     argument vectors.  The child process will create this itself.
     However, we do create the process's environment vectors.
     From now on, this is the point of no return.  */
#ifdef DEBUG
  __os_print ("-- execve: proc->envp="); __os_prhex ((int) process->envp); __os_nl ();
  __os_print ("-- execve: proc->argv="); __os_prhex ((int) process->argv); __os_nl ();
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

#ifdef DEBUG
  __os_print ("-- execve: re-create proc->envp and proc->argv\r\n");
#endif

  /* Count new environment variable vector length.  */
  x = -1;
  while (envp[++x])
    ;

  process->envc = x;
  if (process->envc)
    {
      /* Make a copy of the command line arguments.  */
      process->envp = (char **) malloc ((process->envc + 1)
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

#ifdef DEBUG
  __os_print ("-- execve: proc->envc="); __os_prhex ((int) process->envc); __os_nl();
  __os_print ("-- execve: proc->argc="); __os_prhex ((int) process->argc); __os_nl();
#endif

  /* If the cli is >= MAXPATHLEN, we will need the aid of DDE utils.  */
  if (strlen (cli) >= MAXPATHLEN && set_dde_cli (cli) < 0)
    return -1;

  /* File descriptors open in the existing process image remain open
     in the new process image, unless they have the 'FD_CLOEXEC'
     flag set.  O_EXECCL is an alias for FD_CLOEXEC.  */
  for (x = 0; x < MAXFD; x++)
    if (process->fd[x].__magic == _FDMAGIC && process->fd[x].dflag & O_EXECCL)
      close (x);

  process->status.has_parent = 0;

#ifdef DEBUG
  __debug ("-- execve: process after new argv and envp setup");
  __os_print ("__rwlimit: "); __os_prhex ((unsigned int) __rwlimit); __os_nl ();
  __os_print ("__stack_limit: "); __os_prhex ((unsigned int) __stack_limit);
  __os_nl ();
#endif

  /* Force a malloc trim to reduce memory usage.  malloc() must not be
     called after the __codeshift calculation below since __stack_limit
     could be increased by any call to malloc().  Thus, malloc trim here
     is guaranteed to minimise the malloc heap as best as possible
     before running the child program.  */
  malloc_trim (0);
  __stackalloc_trim ();

  if (((unsigned int) __base & ~0xff) == 0x8000)
    {
      __codeshift = ((char *) __stack - (char *) __stack_limit) - 512 - __exlen;
      /* Heap might not be in a dynamic area (dynamic_num == -1).  */
      __exshift = (__dynamic_num == -1) ? __codeshift : 0;
    }
  else
    __codeshift = __exshift = 0;

  /* __codeshift needs to be > 0, but how much is a suitable minimum ?  */
  if (__codeshift < cli_length)
    return __set_errno (ENOMEM);

#ifdef DEBUG
  __os_print ("__exlen: "); __os_prhex ((unsigned int) __exlen);
  __os_print ("__codeshift: "); __os_prhex ((unsigned int) __codeshift);
  __os_print ("__exshift: "); __os_prhex ((unsigned int) __exshift);
  __os_nl ();
#endif

  /* Restore the original RISC OS environment handlers.  We need to do
     this because we will be physically changing location in RAM.  RISC OS
     will then have a set of invalid environment pointers which would
     break things if something goes wrong from this stage onwards.  */
  __env_riscos ();

#ifdef DEBUG
  __os_print ("-- execve: environment handlers restored\r\n");
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
      ushift (process->envp, variable, code);
      for (i = 0; i < process->envc; i++)
	ushift (process->envp[i], variable, code);
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
	  if (process->tty[i].ptr)
	    ushift (process->tty[i].ptr, variable, code);
	}
      ushift (process->tty, variable, code);

      __exec = (void (*)(char *)) (void *) ((char *) __stack_limit + __codeshift);
    }
  else
    __exec = __exptr;

  /* Set address of proc struct for child. This WILL change soon.  */
  {
    int regs[10];
    char *address;

    address = (char *) process + (process < __stack_limit ? __exshift : 0);
    regs[0] = (int) "UnixLib$env";
    regs[1] = (int) &address;
    regs[2] = 4;
    regs[3] = 0;
    regs[4] = 1;
    __os_swi (OS_SetVarVal, regs);
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
      __os_swi (OS_SynchroniseCodeAreas, regs);
    }
  if (__exshift)
    memcpy ((char *) __lomem + __exshift, (char *) __lomem,
	    (char *) __stack_limit - (char *) __lomem);

  /* Finally call the program.  */
#ifdef DEBUG
  __os_print ("-- execve: about to call:"); __os_print (cli); __os_nl ();
#endif

  __funcall ((*__exec), (cli));
  /* This is never reached.  */
  return 0;
}

void
__exret (void)
{
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
		(char *) __stack_limit - (char *) __lomem);
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
	  if (process->tty[i].ptr)
	    dshift (process->tty[i].ptr, variable, code);
	}
      
      dshift (process->envp, variable, code);
      for (i = 0; i < process->envc; i++)
	dshift (process->envp[i], variable, code);
      process->argc = 0;
      process->argv = NULL;
    }

  /* We don't need to read the current RISC OS environment handlers because
     they should be exactly the same as when we read them at UnixLib
     initialisation time, other than the wimpslot/appspace/himem sizes which
     we don't want to alter anyway.  */

  /* Install the UnixLib environment handlers.  */
  __env_unixlib ();
  __remenv_from_os ("UnixLib$env");

#ifdef DEBUG
  __debug ("-- __exret: process table has just been relocated");
#endif

  __stackfree (__u->cli);

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
  __os_print ("-- __exret: return code="); __os_prdec (i); __os_nl ();
#endif

  if (___vret)
    __funcall ((*___vret), (i));
  else				/* oh fuck... */
    _exit (1);
}
