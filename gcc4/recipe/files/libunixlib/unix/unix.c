/* UnixLib process initialisation and finalisation.
   Copyright (c) 2002, 2003, 2004, 2005 UnixLib Developers.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <swis.h>
#include <termios.h>
#include <time.h>

#include <fpu_control.h>

#include <sys/param.h>
#include <sys/wait.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <unixlib/sigstate.h>
#include <internal/swiparams.h>

/*#define DEBUG 1*/

#include <sys/debug.h>

void (*__atexit_function_array[__MAX_ATEXIT_FUNCTION_COUNT]) (void);
int __atexit_function_count = 0;

static void initialise_unix_io (void);
static void check_fd_redirection (const char *filename,
				  unsigned int fd_to_replace);
static int get_fd_redirection (const char *redir);
static const char *find_terminator (const char *s);
static void get_io_redir (const char *cli);
static int verify_redirection (const char *redirection);
static const char *find_redirection_type (const char *command_line,
					  char redirection_type);
static int convert_command_line (struct proc *process, const char *cli,
				 int cli_size);

static void __badr (void) __attribute__ ((__noreturn__));

/* This is the only file that need ever reference main()
   so to prevent possible compiler errors from sources that
   might include <unixlib/unix.h> we will have the main()
   declaration here.  */
#if defined(PIC)
extern int (*main) (int, char *[], char **);
#else
extern int main (int, char *[], char **);
#endif

/* Only called externally from here - see comment below */
extern int dsp_exit(void);

static struct proc ___u;
struct proc *__u = &___u;	/* current process */

int __escape_disabled;

static void
__badr (void)
{
  __unixlib_fatal ("Bad redirection");
}

/* strtoul to avoid pulling in global verion for small executables.
   This version is limited to base 10, the first character is known to be
   a digit and has less error checking.  */
unsigned int
__decstrtoui (const char *nptr, const char **end)
{
  unsigned int result = 0;

  do
    result = result * 10 + (*nptr++ - '0');
  while (isdigit (*nptr));
  if (end)
    *end = nptr;

  return result;
}

/* strtoul to avoid pulling in global verion for small executables.
   This version is limited to exactly 2 hexadecimal characters.  */
static char
__hexstrtochar (const char *nptr)
{
  char result;

  if (*nptr >= 'A')
    result = (*nptr - 'A' + 10) << 4;
  else
    result = (*nptr - '0') << 4;

  nptr++;

  if (*nptr >= 'A')
    result += (*nptr - 'A' + 10);
  else
    result += (*nptr - '0');

  return result;
}

/* Free any remaining memory and file descriptors associated with a process */
void __free_process(struct __sul_process *process)
{
  struct __sul_process *child = process->children;
  struct __sul_process *next_child;

  /* Close all file descriptors.  */
  if (process->file_descriptors)
    {
      unsigned int i;
      struct __unixlib_fd *file_desc;

      for (i = 0; i < process->maxfd; i++)
	{
	  file_desc = (struct __unixlib_fd *)((char *)(process->file_descriptors) + i * process->fdsize);
	  if (file_desc->devicehandle)
	    __close (file_desc);
	}

      __proc->sul_free (__proc->pid, process->file_descriptors);
      process->file_descriptors = NULL;
    }

  if (process->environ)
    {
      __proc->sul_free (__proc->pid, process->environ);
      process->environ = NULL;
    }

  if (process->console)
    {
      if (__atomic_modify (&(process->console->refcount), -1) == 0)
	__proc->sul_free(__proc->pid, __proc->console);
      process->console = NULL;
    }

  if (process->rs423)
    {
      if (__atomic_modify (&(process->rs423->refcount), -1) == 0)
	__proc->sul_free(__proc->pid, process->rs423);
      process->rs423 = NULL;
    }

  /* Free all zombie children of this process */
  while (child)
    {
      next_child = child->next_child;
      if (child->status.zombie)
	{
	  __free_process (child);
	}
      else
	{
	  /* The child is still running, so we can't free it */
	  child->ppid = 1;
	  child->next_child = NULL;
	}

      child = next_child;
    }

  process->children = NULL;

  /* If this a process struct from a zombie child, then free it */
  if (process->status.zombie)
    __proc->sul_free (__proc->pid, process);
}

/* Initialise the UnixLib world.  Create a new process structure, initialise
   the UnixLib library and parse command line arguments.
   This function is called by __main () in sys.s._syslib.

   No calls to 'brk', 'sbrk' or 'malloc' should occur before calling
   this function.  */
void __unixinit (void)
{
  int __cli_size, cli_size, regs[10];
  char *cli;

#ifdef DEBUG
  debug_printf ("__unixinit: new process\n");
#endif

  /* Calls to 'brk', 'sbrk' or 'malloc' can occur once the call
     to '__resource_initialise' has completed.  */
  __resource_initialise (__u);

  /* Record the initial escape key status. If escape is disabled (as it
     might be if we are being run as an ANSI task from Nettle) then we
     want to ensure the tty driver does not reenable it. */
  __os_byte (0xe5, 0, 0xff, regs);
  __escape_disabled = regs[1];

  /* Initialise the pthread system */
  __pthread_prog_init ();
  __unixlib_signal_initialise (__u);
  /* Initialise ctype tables to the C locale.  */
  __build_ctype_tables (-2);
  /* Define and initialise the Unix I/O.  */
  initialise_unix_io ();
  __stdioinit ();

  /* When the DDEUtils module is loaded, we can support chdir() without
     the RISC OS CSD being changed. When not loaded, chdir() will work by
     changing CSD for all processes.

     IMPORTANT NOTE: because of bugs in DDEUtils' path processing
     we don't set DDEUtils_Prefix at the beginning of each process.
     Symptoms of these bugs are "ADFS::HardDisc4.$ is a directory"
     RISC OS error when Font_FindFont is done for a font not yet in
     the font cache.
     These problems are known to be solved in RISC OS Adjust 1.  */
#if __UNIXLIB_SET_DDEPREFIX == 0
  __u->dde_prefix = __get_dde_prefix ();
#else
  if ((__u->dde_prefix = __get_dde_prefix ()) == NULL)
    {
      regs[0] = (int)"@";
      (void) __os_swi (DDEUtils_Prefix, regs);
    }
#endif

  /* Set up the environment */
  if (__proc->environ)
    {
      /* We have inherited an environment from our parent */
      environ = __proc->environ;
    }
  else
    {
      /* We have no parent, so populate our environment with a subset of the
	 RISC OS global environment. */
      char *call = NULL;
      int environlen = 10;
      int environentries = 0;

      environ = malloc (environlen * sizeof (char *));
      if (environ == NULL)
	__unixlib_fatal ("Cannot allocate memory for environ");

      do
	{
	  char *value = __getenv_from_os ("*", NULL, 0, &call);
	  if (value)
	    {
	      if (environentries >= environlen)
		{
		  environlen += 10;
		  environ = realloc (environ, environlen * sizeof (char *));
		  if (environ == NULL)
		    __unixlib_fatal ("Cannot allocate memory for environ");
		}
	      environ[environentries++] = value;
	    }
	}
      while (call);

      /* Terminate the list and shrink it to the exact size needed */
      environ = realloc (environ, (environentries + 1) * sizeof (char *));
      if (environ == NULL)
	__unixlib_fatal ("Cannot allocate memory for environ");
      environ[environentries] = NULL;
    }

  /* Get command line.  __unixlib_cli's pointing to the command line block
     returned by OS_GetEnv in __main ().  */
  __cli_size = strlen (__unixlib_cli);

#ifdef DEBUG
  debug_printf ("__unixinit (getting cli) __unixlib_cli=%s\n",
		__unixlib_cli);
#endif

  /* Since the command line limit of RISC OS is only 255 characters,
     extra data can be passed through a DDEUtils buffer setup by the calling
     application.  If such command line information exists, then obtain
     it and append it to our existing command line.  */
  if (__os_swi (DDEUtils_GetCLSize, regs) != NULL)
    regs[0] = 0;

  cli_size = __cli_size + regs[0];
  cli = malloc (cli_size + 2);
  if (cli != NULL)
    {
      memcpy (cli, __unixlib_cli, __cli_size);
      cli[__cli_size] = '\0';
      if (__cli_size < cli_size)
	{
	  _kernel_oserror *err;

	  /* Append DDEUtils command line.  */
	  cli[__cli_size] = ' ';
	  regs[0] = (int) cli + __cli_size + 1;
	  if ((err = __os_swi (DDEUtils_GetCl, regs)) != NULL)
	    {
	      __unixlib_fatal ("Cannot get command line");
	    }
	}
    }

  /* Set command line length to zero otherwise the next process
     will also get it.  */
  regs[0] = 0;
  (void) __os_swi (DDEUtils_SetCLSize, regs);

  if (cli == NULL)
    __unixlib_fatal ("Command line too long (not enough memory)");

  /* Find any environment variables that may influence parts of UnixLib's
     decision making.  */
  __runtime_features (cli);

#ifdef DEBUG
  debug_printf ("__unixinit: create argv: cli_size=%d\n", cli_size);
#endif
  /* Parse the command line, looking for I/O redirection.  */
  get_io_redir (cli);
  /* Convert the command line to a argv block.  */
  convert_command_line (__u, cli, cli_size);
  free (cli);

  /* The libm code requires strict IEEE double precision arithmetic.
     The compiler generates code assuming that the AC bit is turned on.
     See fpu_control.h for further information.  */
  /* _FPU_SETCW (_FPU_IEEE); */
  _FPU_SETCW (_FPU_DEFAULT);

#ifdef DEBUG
  __debug ("__unixinit: process creation complete");
#endif
}

int _main (void)
{
  /* Enter the user's program. For compatibility with Unix systems,
     pass the 'environ' variable as a third argument.

     This copy of the environment will not get updated by getenv
     or setenv */

#if defined(PIC)
  /* Validate the main function pointer (shared library only) */
  if (main == NULL)
    __unixlib_fatal ("Pointer to main function is NULL");
#endif

  return main (__u->argc, __u->argv, environ);
}

void
exit (int status)
{
  /* Execute atexit functions in the reverse order of their
     registration.
     Although this accesses globals, we don't want to disable threads
     whilst calling the functions. Anything calling atexit from an already
     registered function is on dodgy ground anyway. */
  while (__atexit_function_count-- > 0)
    {
      __funcall ((*__atexit_function_array[__atexit_function_count]), ());
    }

  _Exit (status);
}

/* ISOC99: Terminate the program with `status' without calling any of
   the functions registered with `atexit'.  */
void _Exit (int status)
{
  /* Close all open streams, write out buffered output data and
     delete tmpfile() temporary files.  */
  __stdioexit ();

  /* Only return codes between 0 and 127 are valid.  128 to 255 are
     reserved for the run-time library internals.  */
  status = status & 0x7f;

  _exit (__W_EXITCODE (status, 0));
}

/* Final process termination. 'return_code' is a 16-bit
   encoded quantity as defined by <sys/wait.h>.
   Also called via __unixlib_fatal() so don't assume too much.  */
void
_exit (int return_code)
{
  int status;

  /* Interval timers must be stopped.  */
  __stop_itimers ();

  /* pthread timers must be stopped */
  if (__pthread_system_running)
    {
      __pthread_stop_ticker ();
      __pthread_system_running = 0;
    }

  /* De-register with DigitalRenderer in case of an exception */
  dsp_exit();

  /* Convert the 16-bit return code into an 8-bit equivalent
     for compatibility with RISC OS.

     The exit code:
	if bit 7 == 0
	   bits 0..6 = normal exit code
	else
	  {
	     bits 0..5 = signal number that terminated the process.
	     bit 6 == 1 if process core dumped
	  }
  */

  if (WIFSIGNALED (return_code))
    {
      /* Process terminated with a signal.  */
      status = WTERMSIG (return_code);
      __proc->status.return_code = 0;
      __proc->status.signal_exit = 1;
      __proc->status.signal = status;
      status |= (1 << 7);

      if (WCOREDUMP (return_code))
	{
	  status |= (1 << 6);
	  __proc->status.core_dump = 1;
	}
      else
	{
	  __proc->status.core_dump = 0;
	}
    }
  else
    {
      status = WEXITSTATUS (return_code);
      __proc->status.return_code = status;
      __proc->status.core_dump = 0;
      __proc->status.signal_exit = 0;
    }

  /* Reset the DDEUtils' Prefix variable to the value at startup.  */
  if (__u->dde_prefix)
    {
      int regs[10];

      regs[0] = (int) __u->dde_prefix;
      (void) __os_swi (DDEUtils_Prefix, regs);
    }

  /* Re-enable Escape (in case SIGINT handler fired in ttyicanon) */
  if (!__escape_disabled)
    __os_byte (229, 0, 0, NULL);

  __free_process (__proc);
  __dynamic_area_exit ();
  __env_riscos ();

#ifdef PIC
  {
    /* Tell the Shared Object Manager that the current client has exited */
    int regs[10];

    (void) __os_swi(SOM_DeregisterClient, regs);
  }
#endif

#ifdef DEBUG
  debug_printf ("__exit(): Calling sul_exit with return code=%d\n", status);
#endif
  __proc->sul_exit (__proc->pid, status);
}

int
__alloc_file_descriptor (int start)
{
  unsigned int i;

  PTHREAD_UNSAFE

  /* Look for a spare file descriptor.  */
  for (i = start; i < __proc->maxfd; i++)
    if (getfd (i)->devicehandle == NULL)
      {
#ifdef DEBUG
	debug_printf ("__alloc_file_descriptor: found free descriptor %d\n",
		      i);
#endif
	return i;
      }

  /* Set errno to indicate the unavailability of file handles.  */
  return __set_errno (EMFILE);
}

void
__stop_itimers (void)
{
  struct itimerval new_timer;

  /* Interval timers are not implemented in task windows nor in WIMP
     programs so we don't need to stop them.   Note that when
     __taskwindow == 1 => __taskhandle != 0 but not necessary vice-
     versa so the test on __taskhandle is enough.  */
  if (__taskhandle != 0)
    return;

  /* Stop all interval timers.  */
  new_timer.it_interval.tv_sec = 0;
  new_timer.it_interval.tv_usec = 0;
  new_timer.it_value.tv_sec = 0;
  new_timer.it_value.tv_usec = 0;
  setitimer (ITIMER_REAL, &new_timer, 0);
  setitimer (ITIMER_VIRTUAL, &new_timer, 0);
  setitimer (ITIMER_PROF, &new_timer, 0);
}

/* Define and initialise the Unix I/O.  */
static void
initialise_unix_io (void)
{
  if (__proc->file_descriptors == NULL)
    {
      _kernel_oserror *err;
      int regs[10];
      unsigned int i;

      __proc->file_descriptors = __proc->sul_malloc (__proc->pid,
						     __proc->maxfd * __proc->fdsize);
      if (__proc->file_descriptors == NULL)
	__unixlib_fatal ("Cannot allocate file descriptor memory");

      /* Set all file descriptors to unallocated status.  */
      for (i = 0; i < __proc->maxfd; i++)
	getfd (i)->devicehandle = NULL;

      /* These are guaranteed to be the first files opened. stdin, stdout
	 and stderr will receive file descriptor numbers 0, 1 and 2
	 respectively.  */

      /* Open a file descriptor for reading from the tty (stdin)
	 and writing to the tty (stdout) when there is no CLI
	 redirection done by RISC OS, otherwise take RISC OS own
	 files handles as basis for stdin and/or stdout.  */
      regs[1] = regs[0] = -1;
      err = __os_swi (OS_ChangeRedirection, regs);
      if (err != NULL)
	__unixlib_fatal (err->errmess);
      if ((regs[0] ? __open_fh (STDIN_FILENO, regs[0], O_RDONLY, 0777)
                   : __open_fn (STDIN_FILENO, "/dev/tty", O_RDONLY, 0777)) < 0)
	__unixlib_fatal ("Cannot open stdin");
      if ((regs[1] ? __open_fh (STDOUT_FILENO, regs[1], O_WRONLY | O_CREAT, 0666)
                   : __open_fn (STDOUT_FILENO, "/dev/tty", O_WRONLY | O_CREAT, 0666)) < 0)
	__unixlib_fatal ("Cannot open stdout");

      /* Duplicate the file descriptor for stdout, to create a suitable
	 file descriptor for stderr.  */
      if (fcntl (STDOUT_FILENO, F_DUPFD, STDERR_FILENO) < 0)
	__unixlib_fatal ("Cannot open stderr");
    }
}

/* Attempt to re-direct a file descriptor based on the file descriptor
   number that was passed on the command line.  */
static void
check_fd_redirection (const char *filename, unsigned int fd_to_replace)
{
  /* <&- means shut.
     <&[fd] would mean dup this fd.  */

  if (*filename == '-')
    {
      close (fd_to_replace);
      return;
    }

  if (isdigit (*filename))
    {
      unsigned int dup_fd = __decstrtoui (filename, NULL);

      if (dup_fd >= __proc->maxfd)
	__badr ();

      if (dup_fd != fd_to_replace)
	{
	  /* Duplicate the file descriptor.  */
	  dup2 (dup_fd, fd_to_replace);
	}
    }
  else
    __badr ();
}

/* Translate the file descriptor part of [fd]>filename into a number.  */
static int
get_fd_redirection (const char *redir)
{
  unsigned int fd, multiplier;

  /* On entry, *redir will point to either a left or right chevron.  */
  redir--;

  /* Since we're reading the number backwards, we will use a multiplier
     to correctly position the digit into the number.  */
  multiplier = 1;

  fd = 0;
  while (*redir && isdigit (*redir))
    {
      fd += (*redir - '0') * multiplier;
      multiplier *= 10;
      redir--;
    }
  if (fd >= __proc->maxfd)
    __badr ();

  return fd;
}

/* Search through a string and look for a terminator that is one of
   NULL, space, newline or carriage-return.  */
static const char *
find_terminator (const char *s)
{
  while (*s != '\0' && *s != ' ' && *s != '\r' && *s != '\n')
    s++;

  return s;
}


static void check_io_redir (const char *p, int fd, int mode)
{
  char fn[256];
  char *ptr = fn;
  const char *space;

  if (isdigit (p[-1]))
    fd = get_fd_redirection (p);
  else if (p[-1] == '>' && isdigit (p[-2]))
    fd = get_fd_redirection (p - 1);

#ifdef DEBUG
  debug_printf ("check_io_redir: redirecting fd %d\n", fd);
#endif

  /* Skip any whitespace that precedes the filename e.g '< filename'.  */
  p++; /* skip the '<' */
  while (*p && isspace (*p))
    p++;

  space = find_terminator (p);
  strncpy (fn, p, space - p);
  /* Zero terminate the filename.  */
  fn[space - p] = '\0';
#ifdef DEBUG
  debug_printf ("check_io_redir: filename='%s', mode=%08x\n",
		fn, mode);
#endif

  /* Check the >& construct.  */
  if (*ptr == '&')
    check_fd_redirection (ptr + 1, fd);
  else
    {
      /* Close the file descriptor, if it was defined and open.  */
      if (!BADF (fd))
	__close (getfd (fd));

      fd = __open_fn (fd, ptr, mode, 0666);
      if (fd < 0)
	{
	   char failure[300];
	   snprintf (failure, sizeof (failure),
		     "Cannot open %s for I/O redirection", ptr);
	   __unixlib_fatal (failure);
	}

      if (((mode & O_WRONLY) || (mode & O_RDWR)) && ! (mode & O_TRUNC))
	/* Seek to the end of the file, because we are appending.  */
	lseek (fd, 0, SEEK_END);
    }
}

static void get_io_redir (const char *cli)
{
  const char *p = cli;
  int mode;

  /* By default, we redirect file descriptor 0 (stdin).  */
#ifdef DEBUG
  debug_printf ("get_io_redir: checking <\n");
#endif
  while ((p = find_redirection_type (p, '<')))
    {
      /* A redirector `<>' means read/write.  */
      mode = (p[1] == '>') ? O_RDWR : O_RDONLY;
      check_io_redir (p, STDIN_FILENO, mode);
      p++;
    }

  p = cli;
  /* By default, we redirect file descriptor 1 (stdout).  */
  mode = O_WRONLY | O_CREAT;
#ifdef DEBUG
  debug_printf ("get_io_redir: checking >\n");
#endif
  while ((p = find_redirection_type (p, '>')) != NULL)
    {
      if (p[-1] != '<')
	{
	  /* p might point to:
	     1. >
	     2. >>
	     3. 2>
	     4. 2>>
	  */
	  if (isdigit (p[0]))
	    {
	      p++;
	      if (p[0] == '>' && p[1] == '>')
		p++, check_io_redir (p, STDOUT_FILENO, mode);
	      else if (p[0] == '>')
		check_io_redir (p, STDOUT_FILENO, mode | O_TRUNC);
	    }
	  else
	    {
	      if (p[0] == '>' && p[1] == '>')
		p++, check_io_redir (p, STDOUT_FILENO, mode);
	      else
		check_io_redir (p, STDOUT_FILENO, mode | O_TRUNC);
	    }
	}
      p++;
    }
}

/* Verify that 'redirection' is actually pointing to a redirection
   operator. Return 0 if it is not, return 1 if it is.  */
static int
verify_redirection (const char *redir)
{
  int x;

#ifdef DEBUG
  debug_printf ("verify_redirection: %s\n", redir);
#endif

  /* So we've found a re-direction operator.  We must watch out
     for the filename <spec$dir>.file since these chevrons so not
     refer to re-direction.  */

  /* Check for special 'give away' character sequences that we can
     be sure are re-direction operators:
	1. >>
	2. >&[a number]
	3. <&[a number]
	4. >&-
	5. <&-
	6. <>
	7. >[space]
	8. <[space]
	9. [a number]>
	10. [a number]<
	11. [a number]<>

     Without the [a number] >& can refer to the start of a valid
     RISC OS pathname.  */
  if (redir[0] == '>' && redir[1] == '>')
    return 1;

  if (redir[1] == '&'
      && (redir[2] == '-' || isdigit (redir[2])))
    return 1;


  /* This should match:
     "2>foo"
     "2<foo"
  */
  x = 0;
  if (isdigit (redir[x]))
    {
      while (redir[x] && isdigit (redir[x]))
	x++;

      if (redir[x] == '<' || redir[x] == '>')
	return 1;
    }

  if (redir[0] == '<')
    {
      const char *t = redir;

      /* Check for a construct like <<spec$dir>.file.  or for `<>'  */
      if (redir[1] == '<' || redir[1] == '>')
	return 1;

      while (*t && *t != ' ' && *t != '>')
	t++;

      /* If there's no right chevron on the rest of the cli, then we
	 must have a proper re-direction operator.  */
      if (*t == '\0' || *t == ' ')
	return 1;

      /* It was a RISC OS filename.  */
      return 0;
    }

  /* The output re-direction operator (right chevron) is very easy
     to check.  The only position in which it can appear is in the
     middle of a RISC OS filename e.g. <spec$dir>.file.  If there
     is a space before this chevron, then it *must* be referring
     to a re-direction operator.

     An array index of -1 is safe here, because it is impossible for
     a right chevron to appear at the start of the command line
     (unless you are the idiot who regularly uses > for a filename). */
  if (redir[0] == '>' && redir[-1] == ' ')
    return 1;

  return 0;
}

static const char *
find_redirection_type (const char *cmdline, char redirection_type)
{
  const char * const org_cmdline = cmdline;

  while (1)
    {
      /* Look for redirection operator `redirection_type'.  */
      while (*cmdline && *cmdline != redirection_type)
	{
	  while (*cmdline && *cmdline != redirection_type
		 && *cmdline != '"' && *cmdline != '\'')
	    cmdline++;

	  if (*cmdline == '"')
	    {
	      /* Skip over anything in double quotes */
	      cmdline++;
	      while (*cmdline && *cmdline != '"')
		cmdline++;
	      if (*cmdline == '"')
		cmdline++;
	    }
	  else if (*cmdline == '\'')
	    {
	      /* Skip over anything in single quotes, unless it is an
		 apostrophe in a filename */
	      cmdline++;
	      if (cmdline[-2] == ' ')
		{
		  while (*cmdline && *cmdline != '\'')
		    cmdline++;
		  if (*cmdline == '\'')
		    cmdline++;
		}
	    }
	}

      if (*cmdline == '\0')
	/* Operator `redirection_type' doesn't exist.  */
	return NULL;

      /* Cope with "2> foobar" */
      while (cmdline != org_cmdline && isdigit (cmdline[-1]))
	cmdline --;

      if (verify_redirection (cmdline))
	return cmdline;
      /* Skip a character otherwise we loop on <foo$bar>.  */
      cmdline ++;
    }
}

/* Convert a command line that is contained as a single string, with
   arguments deliminated by spaces, into a zero terminated string with
   index markers.  */
static int
convert_command_line (struct proc *process, const char *cli, int cli_size)
{
  int argc;
  char **argv, *temp, *p;

  /* A temporary buffer for the command line arguments, holds a
     particular argument prior to it being added to the argv array.  */
  temp = (char *) malloc (cli_size + 1);

  argc = 0;
  argv = (char **) malloc ((argc + 2) * sizeof (char *));
  if (temp == NULL || argv == NULL)
    __unixlib_fatal ("Cannot allocate memory for main() parameters");
  while (*cli)
    {
      /* Set to 1 if we are storing an empty argument, for example
	 -classpath ''  */
      int empty_arg = 0;

      /* Skip any white space.  */
      while (isspace (*cli))
	cli++;

      /* Finish if we have reached the end of the command line.  */
      if (*cli == '\0')
	break;

      p = temp;

      /* We check for *cli != NULL here because of a command line like:
	    main 5>fred

	 verify_redirection will catch this and move cli onto the
	 terminating character (in this case, the null).  NULL is not
	 a space, so we'd start writing garbage into the argv array.  */
      while (*cli && ! isspace (*cli))
	{
	  /* Don't check for a redirection operator in the first argument
	     because that is the program name.  */
	  if (argc != 0 && verify_redirection (cli))
	    {
	      /* We don't add redirection operators to the argument list.
		 So skip all characters up until the next real argument.  */

	      /* We have to watch out for stuff like:
		   1. <> filename
		   2. < filename
		   3. > filename
		   4. >> filename.  */

	      /* Just incrementing cli by one here converts a <> filename
		 into a > filename.  We're not interested in the re-direction
		 stuff anyway.  */
	      while (*cli && isdigit (*cli))
		cli++;

	      if ((cli[0] == '<' || cli[0] == '>') && cli[1] == '>')
		cli++;

	      if (cli[1] == ' ')
		{
		  /* Skip the chevron.  */
		  cli++;

		  /* Skip any initial whitespace after the chevron.  */
		  while (*cli && isspace (*cli))
		    cli++;
		}

	      /* By the time we've reached here, we should have >filename
		 or just filename.  */

	      /* Skip the filename bit after the chevron.  */
	      while (*cli && !isspace (*cli))
		cli++;
	    }
	  else if (*cli == '\\')
	    {
	      /* Escape character.  */
	      switch (*++cli)
		{
		case '\0':
		  break;

		case 'x':
		  if (cli[1] == '\0' || cli[2] == '\0')
		    break;

		  *p++ = __hexstrtochar (cli + 1);
		  cli += 3;
		  break;
		default:
		  *p++ = *cli++;
		}
	    }
	  else if (*cli == '\"')
	    {
	      /* Argument is enclosed in double-quotes.  */

	      /* Skip first quote.  */
	      cli++;
	      /* Keep copying characters into the argv_string until we reach
		 the terminating quotation mark.  */
	      while (*cli != '\0' && *cli != '\"')
		{
		  /* Look out for escape characters.  */
		  if (*cli == '\\')
		    {
		      switch (*++cli)
			{
			case '\0':
			  break;

			case 'x':
			  if (cli[1] == '\0' || cli[2] == '\0')
			    break;

			  *p++ = __hexstrtochar (cli + 1);
			  cli += 3;
			  break;
			default:
			  *p++ = *cli++;
			}
		    }
		  else
		    *p++ = *cli++;
		}
	      /* If we've finished on the quotation mark then skip over it.  */
	      if (*cli == '\"')
		{
		  if (cli[-1] == '\"')
		    empty_arg = 1;
		  cli++;
		}
	    }
	  else if (*cli == '\'' && isspace(cli[-1]))
	    {
	      /* The argument is contained within single quotes. We also
		 hackily check for a space before it - this avoids problems
		 with filenames with apostrophes in them.  */
	      cli++;
	      while (*cli != '\0' && *cli != '\'')
		{
		  if (cli[0] == '\\' && cli[1] == 'x' && cli[2] && cli[3])
		    {
		      /* Sort out escape sequences that are added by UnixLib */
		      *p++ = __hexstrtochar (cli + 2);
		      cli += 4;
		    }
		  else
		    *p++ = *cli++;
		}
	      /* If we've finished on the single quote mark, then skip it.  */
	      if (*cli == '\'')
		{
		  if (cli[-1] == '\'')
		    {
		      /* The argument was given as an empty argument.
			 A circumstance like this occurs with commands like
			    -classpath ''
			 We must therefore store an empty argument.
		      */
		      empty_arg = 1;
		    }
		  cli++;
		}
	    }
	  else
	    *p++ = *cli++;
	}

      *p = '\0';
      /* We have now reached a space delimiter.  Add the argument to
	 the argv list, if one was actually made.  */
      if (p != temp || empty_arg)
	{
	  argc ++;
	  argv = (char **) realloc (argv, (argc + 1) * sizeof (char *));
	  argv[argc - 1] = strdup (temp);
	  if (argv == NULL || argv[argc - 1] == NULL)
	    __unixlib_fatal ("Cannot allocate memory for main() parameters");
	}
  }

  /* Set the last item to NULL */
  argv[argc] = NULL;

  free (temp);

  /* Convert the process filename, either to a RISC OS full path, either
     to an Unix full path.  Note that there is a fundamental problem when
     you start a process via a temporary FS selection and that the current
     FS get taken instead.  Use "/ADFS::MyDisc.$.my_risc_os_process" or
     "run ADFS::MyDisc.$.my_risc_os_process" instead of
     "ADFS::MyDisc.$.my_risc_os_process" to avoid this problem.
     The path isn't converted if there is no . in it, as that indicates
     that it is something on Run$Path and so canonicalising it is not a
     good idea.  */
  if (strchr(argv[0], '.'))
    {
      int regs[10];
      char *new_argv0, *new_uargv0;
      _kernel_oserror *err;
      int filetype;

      regs[0] = 37;
      regs[1] = (int)argv[0];
      regs[5] = regs[4] = regs[3] = regs[2] = 0;
      if ((err = __os_swi(OS_FSControl, regs)) != NULL)
       {
         __ul_seterr (err, 0);
         __unixlib_fatal ("Cannot convert process filename");
       }
      if (regs[5] > 0)
       __unixlib_fatal ("Cannot convert process filename");

      if ((new_argv0 = malloc(1 - regs[5])) == NULL)
       __unixlib_fatal ("Cannot allocate memory to convert process filename");

      regs[0] = 37;
      regs[1] = (int)argv[0];
      regs[2] = (int)new_argv0;
      regs[4] = regs[3] = 0;
      regs[5] = 1 - regs[5];
      if ((err = __os_swi(OS_FSControl, regs)) != NULL)
       {
         __ul_seterr (err, 0);
         __unixlib_fatal ("Cannot convert process filename");
       }

      if (__os_file (OSFILE_READCATINFO, new_argv0, regs) != NULL
         || regs[0] != 1)
       {
#ifdef DEBUG
         __os_print ("WARNING: cannot stat() process filename\r\nDid you use a temporary FS used to startup? If so, better use '*run' instead.");
         __os_nl ();
#endif
         filetype = __RISCOSIFY_FILETYPE_NOTFOUND;
       }
      else
       filetype = ((regs[2] & 0xfff00000U) == 0xfff00000U) ? (regs[2] >> 8) & 0xfff : __RISCOSIFY_FILETYPE_NOTFOUND;

      /* Convert to Unix full path, if needed.  */
      if ((new_uargv0 = __unixify_std (new_argv0, NULL, 0, filetype)) == NULL)
       __unixlib_fatal ("Cannot allocate memory to convert process filename");

      free(new_argv0);
      free(argv[0]);
      argv[0] = new_uargv0;
    }

  process->argc = argc;
  process->argv = argv;

  return argc;
}
