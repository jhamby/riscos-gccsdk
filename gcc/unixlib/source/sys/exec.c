/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/exec.c,v $
 * $Date: 2004/12/26 10:45:20 $
 * $Revision: 1.18 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: exec.c,v 1.18 2004/12/26 10:45:20 peter Exp $";
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
#include <pthread.h>

/* #define DEBUG 1 */


/* Setup the extended command line by sending all the arguments to the
   program via DDEUtils module.  */
static int
set_dde_cli (char *cli)
{
  int regs[10];
  char *temp;

  /* Skip program name.  */
  for (temp = cli; *temp != '\0' && *temp != ' '; ++temp)
    ;
  /* We know there is exactly one space after the program name.  */
  if (*temp == ' ')
    ++temp;

  /* Check that the actual command is not greater than
     RISC OS's maximum path length.  */
  if ((temp - cli) >= MAXPATHLEN)
    return __set_errno (E2BIG);

  /* Set the command line size within DDEUtils.  */
  regs[0] = strlen (temp) + 1;
  if (__os_swi (DDEUtils_SetCLSize, regs))
    /* We're buggered if DDEUtils isn't on this system.  */
    return __set_errno (E2BIG);

  regs[0] = (int) temp;
  __os_swi (DDEUtils_SetCL, regs);

  /* As we're now passing the CLI arguments via DDEUtils, the CLI
     is now reduced to the program name only.  */
  if (*temp != '\0')
    temp[-1] = '\0';		/* terminate cli.  */
#ifdef DEBUG
  __os_print ("DDEUtils set up\n\r");
#endif
  return 0;
}

/* Execute program `execname' and pass command line arguments `argv'
   and give it the environment `envp'.  */
int
execve (const char *execname, char *const argv[], char *const envp[])
{
  int x, cli_length;
  const char *program_name;
  char *command_line;
  char *cli;
  char pathname[MAXPATHLEN];
  int nasty_hack;
  char *null_list = NULL;
  int scenario;
  char **newenviron = NULL;
  int envlen = 0;


  if (! execname)
    return __set_errno (EINVAL);

  if (! argv)
    argv = &null_list;

#ifdef DEBUG
  __os_print ("-- execve: function arguments\r\n");
  __os_print ("      execname: '"); __os_print (execname); __os_print ("'\r\n");
  for (x = 0; argv[x] != NULL; ++x)
    {
      __os_print ("      argv["); __os_prdec (x); __os_print ("]: ");
      __os_print (argv[x]); __os_nl ();
    }
  for (x = 0; envp[x] != NULL; ++x)
    {
      __os_print ("      envp["); __os_prdec (x); __os_print ("]: ");
      __os_print (envp[x]); __os_nl ();
    }

  __debug ("-- execve: process structure");
#endif

  if (execname[0] == '*' && execname[1] == '\0'
      && argv[1] != NULL)
    scenario = 1;
  else if (!strcmp (execname, "/bin/sh")
	   && argv[1] != NULL && !strcmp (argv[1], "-c")
	   && argv[2] != NULL)
    scenario = 2;
  else
    scenario = 0;

  if (scenario == 0)
    {
      nasty_hack = 0;

      /* If the first argument is the same as the execname then make sure we
         don't end up with it twice on the command line */
      if (argv[0] && strcmp(argv[0], execname) == 0)
        scenario = 1;

      /* Convert the program name into a RISC OS format filename.  */
      program_name = __riscosify_std (execname, 0, pathname,
				      sizeof (pathname), NULL);
      if (program_name == NULL)
	return __set_errno (E2BIG);

      program_name = pathname;		/* This is copied into cli + 1 */
    }
  else
    {
      const char *p;
	  char temp[MAXPATHLEN];

      /* scenario 1 : We've arrived here from a user call to system(),
	 with execname == "*" because on RISC OS the '*' is equivalent
	 to the SHELL of /bin/bash on Unix. If this is the case, then
	 we'll probably find the real program name in argv[1].
	 scenario 2 : "/bin/sh -c xyz" is called and we will execute this
	 as "xyz" (i.e. argv[2]).  */
      p = argv[scenario];

      for (x = 0; x < sizeof(temp) && *p && *p != ' '; /* */)
	temp[x++] = *p++;
      if (x == sizeof(temp))
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

#ifdef DEBUG
  __os_print ("-- execve: program_name: "); __os_print (program_name);
  __os_nl ();
#endif

#ifdef DEBUG
  __os_print ("-- execve: building command line\r\n");
#endif

  /* Calculate the length of the command line.  */
  cli_length = strlen (program_name) + 1;
  for (x = scenario; argv[x] != NULL; ++x)
    {
      const char *p;
      int space = 0;
      /* We must add extra characters if the argument contains spaces (wrap
         argument in quotes) and quotes (must add backslash) and
         inverted commas (must add backslash) and control chars
         (change to \xXX).  */
      p = argv[x];
      if (nasty_hack && x == scenario)
	p += nasty_hack;
      for (/* */; *p; ++p)
        {
          if (isspace (*p))
            space = 1;
	  if ((!nasty_hack || x != scenario) && (*p == '\"' || *p == '\''))
            cli_length ++;
          if (*p == 127 || *p < 32)
            cli_length +=3;
          cli_length ++;
        }

      /* Account for quotes around argument but only if our hack isn't
         in place.  */
      if (space && ! nasty_hack)
        cli_length += 2;

      cli_length ++;
    }

#ifdef DEBUG
  __os_print ("-- execve: cli_length = "); __os_prdec (cli_length);
  __os_nl ();
#endif

  /* SUL will free cli for us when we call sul_exec */
  cli = __proc->sul_malloc (__proc->pid, cli_length + 1);
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
    for (x = scenario; argv[x] != NULL; ++x)
      {
        char *p = argv[x];
        int contains_space = 0;

	if (nasty_hack && x == scenario)
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
	if (nasty_hack && x == scenario)
	  p += nasty_hack;
	for (/* */; *p; ++p)
	  {
	    /* If character is a " or a ' then preceed with a backslash.  */
	    if ((!nasty_hack || x != scenario) && (*p == '\"' || *p == '\''))
	      *command_line ++ = '\\';

	    if (*p == 127 || *p < 32)
	      {
		sprintf (command_line, "\\x%.2X", *p);
		command_line += 4;
	      }
	    else
	      *command_line ++ = *p;
	  }
	if (contains_space)
	  *command_line ++ = '\"';
	*command_line ++ = ' ';
      }
  command_line[-1] = '\0';

#ifdef DEBUG
  __os_print ("-- execve: cli: "); __os_print (cli); __os_nl ();
#endif

  /* If the cli is >= MAXPATHLEN, we will need the aid of DDEUtils.  */
  if (cli_length >= MAXPATHLEN && set_dde_cli (cli) < 0)
    {
      __proc->sul_free (__proc->pid, cli);
      return -1;
    }

  if (envp)
    {
      int enventries;
      char *offset;

      envlen = sizeof (char *);

      /* Count new environment variable vector length.  */
      for (x = 0; envp[x] != NULL; x++)
        envlen += sizeof (char *) + strlen (envp[x]) + 1;

      enventries = x;

      /* The length must be word aligned */
      envlen = (envlen + 3) & ~3;

      newenviron =  __proc->sul_malloc (__proc->pid, envlen);
      if (newenviron == NULL)
        {
          __proc->sul_free (__proc->pid, cli);
          return __set_errno (ENOMEM);
        }

      /* Copy the environment */
      offset = (char *)(newenviron + enventries + 1);
      for (x = 0; x < enventries; x++)
        {
          int len = strlen (envp[x]) + 1;
          newenviron[x] = offset;
          memcpy (offset, envp[x], len);
          offset += len;
        }
      newenviron[enventries] = NULL;

    }

  /* From this point onwards we cannot return an error */


#if __UNIXLIB_FEATURE_PTHREADS
  /* All threads are terminated on an exec call.
     Destructor functions are not called. */
  if (__pthread_system_running)
    {
      __pthread_stop_ticker ();
      __pthread_system_running = 0;
    }
#endif

#if __UNIXLIB_FEATURE_ITIMERS
  /* Stop any interval timers that might be running.  Technically
     the new process should inherit the pending alarms.  */
  __stop_itimers ();
#endif

  if (__proc->ppid == 1)
    {
      /* This process doesn't have a parent. Technically, all file descriptors
         that don't have FD_CLOEXEC set should remain open, but if we are
         calling a non-unixlib program then it won't know about them and so
         they will never get closed. */
      __free_process (__proc);
    }
  else
    {
      /* File descriptors open in the existing process image remain open
         in the new process image, unless they have the 'FD_CLOEXEC'
         flag set.  O_EXECCL is an alias for FD_CLOEXEC.  */
      for (x = 0; x < __proc->maxfd; x++)
        if (getfd (x)->devicehandle && (getfd (x)->dflag & O_EXECCL))
          close (x);

      if (__proc->environ)
        __proc->sul_free (__proc->pid, __proc->environ);
    }

  __proc->environ = newenviron;
  __proc->environ_size = envlen;

  /* Force a malloc trim to reduce memory usage.  */
  malloc_trim (0);
  __stackalloc_trim ();

  /* If the DAs are being used, then delete the dynamic area. */
  __dynamic_area_exit ();

  /* Restore the RISC OS environment handlers.  This breaks us out
     of the UnixLib world.  */
  __env_riscos ();

  /* Call SUL to execute command.
     (This function never returns, even for a RISC OS built-in
     command.)  */
#ifdef DEBUG
  __os_print ("-- execve: about to call:"); __os_print (cli); __os_nl ();
#endif
  __proc->sul_exec (__proc->pid, cli, __unixlib_stack_limit, __unixlib_stack);


  /* This is never reached.  */
  return 0;
}


