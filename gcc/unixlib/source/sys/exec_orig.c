/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/exec_orig.c,v $
 * $Date: 2000/07/15 14:52:33 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: exec_orig.c,v 1.1.1.1 2000/07/15 14:52:33 nick Exp $";
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/unix.h>
#include <sys/syslib.h>
#include <sys/debug.h>
#include <sys/swis.h>
#include <sys/wait.h>
#include <unixlib/local.h>

#define DEBUG 1

#ifdef DEBUG
#include <sys/os.h>
#include <stdio.h>
#endif


#ifdef DEBUG
#define ushift(p,v,c) ((p) = __ushift((unsigned char *)p,v,c,"(" #p ")"))
#define ushift2(p,v,c,t) ((p) = (t)__ushift((unsigned char *)p,v,c,"(" #p ")"))
#define showmove(s,n,p,r) \
  (os_print(s "("), os_print(n), os_prhex((int)p), os_print(") -> "), \
   os_prhex((int)r), os_nl())
#undef memcpy
#define memcpy(d,s,n) \
  (os_print("memcpy("), os_prhex((int)(d)), os_print(","), os_prhex((int)(s)), \
   os_print(","), os_prhex((int)(n)), os_print(")"), os_nl(), (memcpy)(d,s,n))

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

int
execve (const char *execname, char **argv, char **envp)
{
  struct proc *process = __u;
  int i, length;
  char *s1;
  const char *s3;
  const char *program_name;
  char *command_line;
  void (*__exec) (char *);
  char *cli;
  char pathname[MAXPATHLEN];	/* There is scope to merge this buffer with
				   cli. I don't feel brave enough */

  if (!execname || !argv || !envp)
    return __set_errno (EINVAL);

#ifdef DEBUG
  __debug ("execve: entered function. Parameters are:");
  fprintf (stderr, "------------------------------------------next bit\n");
  fprintf (stderr, "execname = '%s'\n", execname);
  fprintf (stderr, "argv = ");
  i = -1;
  while (argv[++i])
    fprintf (stderr, "argv[%d] = '%s', ", i, argv[i]);
  fprintf (stderr, "\nenvp = ");
  i = -1;
  while (envp[++i])
    fprintf (stderr, "envp[%d] = '%s', ", i, envp[i]);
  fprintf (stderr, "\n");
#endif

  if (*execname != '*')
    {
      program_name = __riscosify_std (execname, 0, pathname,
				      sizeof (pathname), NULL);
      if (program_name == NULL)
	return __set_errno (E2BIG);

      length = program_name - pathname + 1;	/* strlen + 1 */
      program_name = pathname;		/* This is copied into cli + 1 */
    }
  else
    {
      program_name = execname;
      length = strlen (program_name) + 1;
    }

#ifdef DEBUG
  os_print ("getting command line length\n\r");
#endif

  /* Move through the command arguments, accumulating the lengths.  */

  for (i = 0; argv[i]; i++)
    length += strlen (argv[i]) + 1;

  cli = (char *) malloc (length) + 1;
  if (cli == NULL)
    return __set_errno (ENOMEM);

  /* If we have a parent, then reallocate the process's argv space.  */
  if (process->status.has_parent)
    {
      /* Have to leave all these valid if any fail midway.
	 Otherwise we'd have a program which may have had its argv
	 truncated  */
      void *temp[4];
      int error = 0;

      /* What's the betting that the optimiser can spot that these are in
	 the same order as the process structure?  */
      temp[0] = malloc ((i + 1) * sizeof (char *));	/* argv  */
      temp[1] = malloc (length);			/* argb  */
      temp[2] = malloc (i * sizeof (size_t));		/* clean_argv  */
      temp[3] = malloc (length);			/* clean_argb  */

      if (!(temp[0] && temp[1] && temp[2] && temp[3]))
	error = ENOMEM;
      else
	{
	  /* Check for overlapping argv arrays.  */
	  length = argv - (char **) temp[0];
	  if (length < 0)
	     length = -length;

	  if (length <= i)
	    error = EINVAL;
	}
      if (error)
	{
	  free (temp[0]);
	  free (temp[1]);
	  free (temp[2]);
	  free (temp[3]);
	  return __set_errno (error);
	}
      free (process->argv);
      free (process->argb);
      free (process->clean_argv);
      free (process->clean_argb);
      process->argv = temp[0];
      process->argb = temp[1];
      process->clean_argv = temp[2];
      process->clean_argb = temp[3];
      /* OK, now we have just trashed the process struct. Surely this is the
	 point of no return?  */
    }

  /* Copy program name into cli and terminate with a space, ready for below.
     Should trim leading space between '*' and command in name.  */
  command_line = cli;

#if 0
  /* XXX If we do this, then we can't call any builtin RISC OS commands.  */
  if (*program_name != '*')
    *command_line++ = '/';
  else
#endif

  /* If we're calling something with system, then we get
     "command args", "*", "command args"
     which we need the child to split into
     "command arg arg arg".

     Aarg. but perl needs * protection in cpp.t
     hack exec to check for argb=="".  */

    /* Skip leading '*'s and ' 's.
       Otherwise OS_CLI will strip these and argb != OS_CLI, which will cause
       the process structure validation to fail.  */
    while (*program_name == '*' || *program_name == ' ')
      program_name++;

  command_line = stpcpy (command_line, program_name);
  *command_line++ = ' ';

  /* If we are a process with no parent, we just verify the command line
     lengths and call the process.  */
  if (!process->status.has_parent)
    {
#ifdef DEBUG
      os_print ("Taking a shortcut\n\r");
#endif
      /* Copy rest of arguments into cli.  */
      if (argv[0])
	for (i = 1; argv[i]; i++)
	  {
	    command_line = stpcpy (command_line, argv[i]);
	    *command_line++ = ' ';
	  }
      command_line[-1] = '\0';

      /* Shutdown unix. This is literally the point of no return.  */
      __reset ();

      /* If the cli is >= MAXPATHLEN, we will need the aid of DDE utils.  */
      if (strlen (cli) >= MAXPATHLEN)
	if (set_dde_cli (cli) < 0)
	  __exit_no_code ();	/* No return possible (see above).  */

      /* If the DAs are being used, then delete the dynamic area first.
	 This is safe to do before the os_cli call since cli is on the stack
	 and hence not in the DA. Firstly, trim the area back with brk().  */
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
    }

  /* If we arrive here, we have a parent process.  */

#ifdef DEBUG
  os_print ("Doing it the long way\n\r");
#endif

  /* Copy name into process->argb.  */
  s1 = stpcpy (process->argb, program_name);

  /* Write cli args, process->argv[] & process->argc */
  if (argv[0])
    {
      s3 = argv[0];
      process->argv[0] = ++s1;
      while ((*s1++ = *s3++))
	;

      for (i = 1; (s3 = argv[i]); i++)
	{
	  process->argv[i] = s1;
	  while ((*s1++ = *command_line++ = *s3++))
	    ;
	  command_line[-1] = ' ';
	}
      length = s1 - process->argv[0];
    }
  else
    {
      i = 0;
      length = 0;
    }

  process->argv[process->argc = i] = NULL;

  /* Terminate cli.  */

  command_line[-1] = '\0';

#ifdef DEBUG
  os_print ("cli set up : "); os_print (cli); os_nl ();
  os_print ("argb setup : "); os_print (process->argb); os_nl ();
#endif


  /* Make the "clean copy" that user program cannot corrupt.  */
  {
    size_t *offset;

    /* Copy command line arguments.  */
    memcpy (process->clean_argb, process->argv[0], length);

    /* Convert pointers into offsets, from end to start.  */
    offset = process->clean_argv + i;
    argv = process->argv + i;

    if (i)
      {
	do
	  {
#ifdef DEBUG
	    os_print ("Copying '");
	    os_print (*(argv-1));
	    os_print ("'\n\r");
#endif
	   } while ((*--offset = (*--argv - process->argv[0])));
           /* argv[0] == argb, hence last offset is 0 and loop terminates.  */
      }
    /* Store length of argb instead of this zero offset.  */
    *offset = length;
  }

  /* If the cli is >= MAXPATHLEN, we will need the aid of DDE utils.  */
  if (strlen (cli) >= MAXPATHLEN)
    if (set_dde_cli (cli) < 0)
      return -1;

#ifdef DEBUG
  __debug ("execve() (argv)");
#endif

  /* Copy environment. Has to be non-null (tested earlier).
     Move through the current environment, accumulating the lengths.  */
  for (i = 0, length = 0; envp[i]; i++)
    length += strlen (envp[i]) + 1;

  /* Allocate the space for the copy of the environment.
     Cannot use realloc, because the old memory may not have come
     from malloc.  */
  process->envb = malloc (length ? length : 1);
  if (process->envb == NULL)
    return __set_errno (ENOMEM);

  process->envp = malloc ((i + 1) * sizeof (char *));
  if (process->envp == NULL)
    return __set_errno (ENOMEM);

  /* Now copy it.  */
  s1 = process->envb;
  for (i = 0; (s3 = envp[i]); i++)
    {
      process->envp[i] = s1;
      while ((*s1++ = *s3++))
	;
    }
  process->envp[i] = NULL;

  /* File descriptors open in the existing process image remain open
     in the new process image, unless they have the 'FD_CLOEXEC'
     flag set.  O_EXECCL is an alias for FD_CLOEXEC.  */
  for (i = 0; i < MAXFD; i++)
    if (process->fd[i].dflag & O_EXECCL)
      close (i);

#if __FEATURE_ITIMERS
  /* Stop any interval timers that might be running.  Technically
     the new process should inherit the pending alarms.  */
  __stop_itimers ();
#endif

  process->status.has_parent = 0;

#ifdef DEBUG
  __debug ("execve()");
#endif

/* check free memory */

#ifdef DEBUG
  os_print ("__rwlimit: ");
  os_prhex ((unsigned int) __rwlimit);
  os_nl ();
  os_print ("__stack_limit: ");
  os_prhex ((unsigned int) __stack_limit);
  os_nl ();
#endif

  /* Force a malloc trim to reduce memory usage. malloc() must not be called
     after the __codeshift calculation below since __stack_limit could be
     increased by any call to malloc(). Thus, malloc trim here is guaranteed
     to minimise the malloc heap as best as possible before running the child
     program.  */
  malloc_trim (0);

  if (((unsigned int) __base & ~0xff) == 0x8000)
    {
      __codeshift = ((char *) __stack - (char *) __stack_limit) - 512 - __exlen;
#ifdef __4K_BOUNDARY
      /* Align down to 4K boundary.  */
      __codeshift = __codeshift & ~4095;
#endif
      if (__dynamic_num == -1)	/* heap not in a dynamic area */
	__exshift = __codeshift;
      else
	__exshift = 0;
    }
  else
    __codeshift = (__exshift = 0);

  /* __codeshift needs to be > 0, but how much is a suitable minimum ?  */
  if (__codeshift < length)
    return __set_errno (ENOMEM);

#ifdef DEBUG
  os_print ("__exlen: ");
  os_prhex ((unsigned int) __exlen);
  os_nl ();
  os_print ("__codeshift: ");
  os_prhex ((unsigned int) __codeshift);
  os_nl ();
  os_print ("__exshift: ");
  os_prhex ((unsigned int) __exshift);
  os_nl ();
#endif

  /* Remove environment handlers.  */
  __restore_calling_environment_handlers ();

#ifdef DEBUG
  os_print ("env handlers restored\r\n");
#endif

  /* Shift __u pointers if necessary.  */
  if (__exshift || __codeshift)
    {
      unsigned int variable = __exshift;
      unsigned int code = __codeshift;

      /* Pointers located between __base and __rwlimit (i.e. the code
	 section of a program) will be relocated using 'code'.

	 Pointers located between __lomem and __break (i.e. the data
	 section of a program) will be relocated using 'variable'.  */
      for (i = 0; i < process->argc; i++)
	ushift (process->argv[i], variable, code);
      ushift (process->argv, variable, code);
      ushift (process->argb, variable, code);
      ushift (process->clean_argv, variable, code);
      ushift (process->clean_argb, variable, code);
      for (i = 0; process->envp[i]; i++)
	ushift (process->envp[i], variable, code);
      ushift (process->envp, variable, code);
      ushift (process->envb, variable, code);
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

/* call it... */

#ifdef DEBUG
  os_print ("Final call : ");
  os_print (cli);
  os_nl ();
#endif

  __funcall ((*__exec), (cli));	/* HACK ME ?? */

/* never reached */

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
      int i;

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
      for (i = 0; process->envp[i]; i++)
	dshift (process->envp[i], variable, code);
      dshift (process->envp, variable, code);
      dshift (process->envb, variable, code);
      dshift (process->clean_argb, variable, code);
      dshift (process->clean_argv, variable, code);
      dshift (process->argb, variable, code);
      dshift (process->argv, variable, code);
      for (i = 0; i < process->argc; i++)
	dshift (process->argv[i], variable, code);
    }

  /* Reinstall handlers.  */

  environment = __c_environment;
  for (i = 0; i < __ENVIRONMENT_HANDLERS; i++)
    __write_environment_handler (i, environment->handlers + i);

  __remenv_from_os ("UnixLib$env");

#ifdef DEBUG
  __debug ("__exret()");
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
  os_print ("return = ");
  os_prhex (i);
  os_nl ();
#endif

  if (___vret)
    __funcall ((*___vret), (i));
  else				/* oh fuck... */
    _exit (1);
}
