/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/unix.c,v $
 * $Date: 2003/04/05 12:16:34 $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: unix.c,v 1.10 2003/04/05 12:16:34 alex Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>

#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <sys/param.h>
#include <swis.h>
#include <sys/wait.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>

#include <pthread.h>

/* #define DEBUG 1 */

#ifdef DEBUG
#include <sys/debug.h>
#endif

#ifndef __GNUC__
#undef attribute
#define attribute(x) /* ignore */
#endif

void (*__atexit_function_array[__MAX_ATEXIT_FUNCTION_COUNT]) (void);
int __atexit_function_count = 0;

static void initialise_process_structure (struct proc *process);
static struct proc *create_process_structure (void);
static void initialise_unix_io (struct proc *process);
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
extern int main (int argc, char *argv[], char **environ);


static void
__badr (void)
{
  __unixlib_fatal ("Bad redirection");
}

/* strtoul to avoid pulling in global verion for small executables.
   This version is limited to base 10, the first character is known to be
   a digit and has less error checking.  */
unsigned int
__decstrtoui (const char *nptr, char **end)
{
  unsigned int result = 0;

  do
    result = result * 10 + (*nptr++ - '0');
  while (isdigit (*nptr));
  if (end)
    *end = (char *) nptr;

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

extern char *__cli;

/* Initialise the UnixLib world.  */
void __unixinit (void)
{
  int i, __cli_size, cli_size = 0, newproc = 0, regs[10];
  char *cli = NULL;

#ifdef DEBUG
  __os_print ("-- __unixinit: __u = "); __os_prhex ((unsigned int) __u);
  if (__u)
    {
      __os_print (", __u->magic="); __os_prhex ((unsigned int) __u->__magic);
    }
  __os_print ("\r\n");
#endif

  if (__u == NULL || __u->__magic != _PROCMAGIC)
    {
#ifdef DEBUG
      __os_print ("-- __unixinit: new process\r\n");
#endif

      /* Flag that we are a new process.  */
      newproc = 1;
      /* Create a process structure if no parent exists or the one
         pointed to by UnixLib$env is invalid.  */
      __u = create_process_structure ();
      if (__u == NULL)
        __unixlib_fatal ("cannot allocate memory for process structure");
      initialise_process_structure (__u);
#if __FEATURE_PTHREADS
      /* Initialise the pthread system */
      __pthread_prog_init ();
#endif
      __resource_initialise (__u);
      __unixlib_signal_initialise (__u);
      /* Initialise ctype tables to the C locale.  */
      __build_ctype_tables (-2);
      /* Define and initialise the Unix I/O.  */
      initialise_unix_io (__u);
      __stdioinit ();
    }
  else
    {
#if __FEATURE_PTHREADS
      /* Initialise the pthread system */
      __pthread_prog_init ();
#endif
      __resource_initialise (__u);
      __unixlib_signal_initialise (__u);
      /* Initialise ctype tables to the C locale.  */
      __build_ctype_tables (-2);
      __stdioinit ();

      /* Inherit environ from parent.  This is our copy, to do with as we
         like except for freeing.  */
      if (! newproc)
        environ = __u->envp;
   }

  if (! environ)
    {
      /* If we are a new process, then we are building a new environment
	 table here.  We can also arrive here if we are a child process
	 and the parent process had nothing in its environment to pass on.  */
      environ = malloc (sizeof (char *));
      if (environ == NULL)
	__unixlib_fatal (NULL);
      *environ = NULL;
    }

  /* Get command line.  */
  __cli_size = strlen (__cli);

#ifdef DEBUG
  __os_print ("-- __unixinit: getting cli: __cli = ");
  __os_print (__cli); __os_print ("\r\n");
#endif

  /* Get extra command line from DDEUtils.  */
  if (__os_swi (DDEUtils_GetCLSize, regs))
    regs[0] = 0;

  cli_size = __cli_size + regs[0];
  cli = malloc (cli_size + 2);
  if (cli != NULL)
    {
      memcpy (cli, __cli, __cli_size);
      cli[__cli_size] = '\0';
      if (__cli_size < cli_size)
        {
          /* Append DDEUtils command line.  */
          cli[__cli_size] = ' ';
          regs[0] = (int) cli + __cli_size + 1;
          __os_swi (DDEUtils_GetCl, regs);
        }
    }

  /* Set command line length to zero otherwise the next process
     will also get it.  */
  regs[0] = 0;
  __os_swi (DDEUtils_SetCLSize, regs);

  if (cli == NULL)
    __unixlib_fatal ("command line too long (not enough memory)");

  /* Find any environment variables.  */
  __runtime_features (cli);

#ifdef DEBUG
  __os_print ("-- __unixinit: create argv: cli_size = ");
  __os_prdec (cli_size); __os_print ("\r\n");
#endif
  /* Parse the command line, looking for I/O redirection.  */
  get_io_redir (cli);
  /* Convert the command line to a argv block.  */
  convert_command_line (__u, cli, cli_size);
  free (cli);

  i = __intenv ("Unix$uid");
  if (i != 0)
    __u->uid = __u->euid = i;

#ifdef DEBUG
  __debug ("__unixinit: process creation complete");
#endif

}

void _main (void)
{
  /* Enter the user's program. For compatibility with Unix systems,
     pass the 'environ' variable as a third argument.

     Make a snapshot of the current environment.  This is necessary
     as doing e.g. a getenv() can update *environ so that this main()
     program is wrongly looking at a free'ed environ ptr array.  */
  int env_var_index;
  char **snapshot_environ;
  for (env_var_index = 0; environ[env_var_index] != NULL; ++env_var_index)
    ;
  snapshot_environ = (char **) malloc((env_var_index + 1) * sizeof (char *));
  if (snapshot_environ == NULL)
    __unixlib_fatal (NULL);
  while (env_var_index >= 0)
    {
      snapshot_environ[env_var_index] = environ[env_var_index];
      --env_var_index;
    }

  main (__u->argc, __u->argv, snapshot_environ);
}

void
exit (int status)
{
  int i;

  /* Execute atexit functions in the reverse order of their
     registration.
     Although this accesses globals, we don't want to disable threads
     whilst calling the functions. Anything calling atexit from an already
     registered function is on dodgy ground anyway. */
  i = __atexit_function_count;
  while (i)
    {
      i--;
      __funcall ((*__atexit_function_array[i]), ());
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
   encoded quantity as defined by <sys/wait.h>.  */
void
_exit (int return_code)
{
  int status;

  /* Interval timers must be stopped.  */
  if (__u)
    __stop_itimers ();

  /* pthread timers must be stopped */
  if (__pthread_system_running)
    {
      __pthread_stop_ticker ();
      __pthread_system_running = 0;
    }

  /* Convert the 16-bit return code into an 8-bit equivalent
     for compatibility with RISC OS.  See sys.c.vfork for
     further information.  */
  if (WIFSIGNALED (return_code))
    {
      status = WTERMSIG (return_code);
      status |= (1 << 7);
      if (WCOREDUMP (return_code))
	status |= (1 << 6);
    }
  else
    {
      status = WEXITSTATUS (return_code);
    }

  /* If we aren't a child process then we can just exit the system.  */
  if (!__u || !__u->status.has_parent || !___vret)
    {
      struct __unixlib_fd *fd = __u->fd;
      int i;

      /* Close all file descriptors.  */
      if (fd)
	for (i = 0; i < MAXFD; i++)
	  if (fd[i].__magic == _FDMAGIC)
	    close (i);

#ifdef DEBUG
      __os_print ("_exit(): Setting return code = ");
      __os_prhex (return_code);
      __os_print ("\r\n");
#endif

      /* OS_Exit with return value 'r'.  This function never returns.  */
      __exit (status);
    }

  /* Due to the change to clock(), this is currently complete crap.  */

#ifdef DEBUG
  __os_print ("_exit(): calling ___vret with return code = ");
  __os_prhex (return_code);
  __os_print ("\r\n");
#endif

  __vret (status);
}


int
__alloc_file_descriptor (void)
{
  struct __unixlib_fd *fd;
  int i;

  PTHREAD_UNSAFE

  fd = __u->fd;

  /* Look for a spare file descriptor.  Suitably unallocated
     ones will have an invalid magic number.  */
  for (i = 0; i < MAXFD; i++)
    if (fd[i].__magic != _FDMAGIC)
      {
#ifdef DEBUG
        __os_print ("__alloc_file_descriptor: found free descriptor ");
        __os_prhex (i);
        __os_nl ();
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
     __taskwindow == 1 => __wimpprogram == 1 but not necessarily vice-
     versa so the test on __wimpprogram is enough.  */
  if (__wimpprogram)
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

static void
initialise_process_structure (struct proc *process)
{
  process->uid = process->euid = 1;
  process->gid = process->egid = 1;
  /* Give the process a process group ID of 2.  */
  process->pgrp = 2;
  /* Give the process a process ID of the monotonic clock.  This is not a
     guaranteed unique number, but it is about the best we can do until a
     module registry is written for UnixLib.  */
  process->pid = clock ();
  process->ppid = 1;
  process->status.tty_type = TTY_CON;
}

/* Initialise a process structure.
   Return NULL on error otherwise return a memory allocated process
   structure.  */
static struct proc *
create_process_structure (void)
{
  struct proc *p;

  /* Reserve some memory for our process structure. If these initialisations
     fail (due to a lack of memory), then we must exit and die.  */
  p = calloc (1, sizeof (struct proc));
  if (p == NULL)
    return NULL;

  p->tty = calloc (MAXTTY, sizeof (struct tty));
  if (p->tty == NULL)
    return NULL;

  /* Set the magic word for our new process structure. We will use
     this when checking the validity of the pointer specified by
     UnixLib$env.  */
  p->__magic = _PROCMAGIC;
  return p;
}

/* Define and initialise the Unix I/O.  */
static void
initialise_unix_io (struct proc *process)
{
  int i;
  const char *tty;

  /* Set all file descriptors to unallocated status.  */
  for (i = 0; i < MAXFD; i++)
    process->fd[i].__magic = 0;

  /* Set the default tty for standard input (stdin), standard output (stdout)
     and standard error (stderr).  Use the filename given by Unix$tty (if
     defined), otherwise default to /dev/tty.  */
  tty = __getenv ("Unix$tty", 0);
  if (tty == NULL)
    tty = "/dev/tty";

  /* These are guaranteed to be the first files opened. stdin, stdout
     and stderr will receive file descriptor numbers 0, 1 and 2
     respectively.  */

  /* Open a file descriptor for reading from the tty (stdin)
     and writing to the tty (stdout).  */
  if (__open (STDIN_FILENO, tty, O_RDONLY, 0777) < 0
      || __open (STDOUT_FILENO, tty, O_WRONLY | O_CREAT, 0666) < 0)
    __unixlib_fatal (NULL);

  /* Duplicate the file descriptor for stdout, to create a suitable
     file descriptor for stderr.  */
  process->fd[STDERR_FILENO] = process->fd[STDOUT_FILENO];
  process->fd[STDERR_FILENO].dflag = 0;
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
      char *end;
      unsigned int dup_fd = __decstrtoui (filename, &end);

      if (dup_fd >= MAXFD)
	__badr ();

      if (dup_fd != fd_to_replace)
        {
          /* Duplicate the file descriptor.  */
          __u->fd[fd_to_replace] = __u->fd[dup_fd];
          __u->fd[fd_to_replace].dflag = 0;
        }
    }
  else
    __badr ();
}

/* Translate the file descriptor part of [fd]>filename into a number.  */
static int
get_fd_redirection (const char *redir)
{
  int fd, multiplier;

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
  if (fd >= MAXFD)
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
  __os_print ("-- check_io_redir: redirecting fd ");
  __os_prdec (fd); __os_print ("\r\n");
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
  __os_print ("-- check_io_redir: filename = '");
  __os_print (fn); __os_print (", mode = ");
  __os_prhex (mode) ; __os_print ("'\r\n");
#endif

  /* Check the >& construct.  */
  if (*ptr == '&')
    check_fd_redirection (ptr + 1, fd);
  else
    {
      /* Close the file descriptor, if it was defined and open.  */
      if (!BADF (fd))
        __close (fd);

      fd = __open (fd, ptr, mode, 0666);
      if (fd < 0)
        {
           char failure[300];

           sprintf (failure, "cannot open %s for I/O redirection", ptr);
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
  __os_print ("-- get_io_redir: checking <\r\n");
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
  __os_print ("-- get_io_redir: checking >\r\n");
#endif
  while ((p = find_redirection_type (p, '>')))
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

#if 0 /*def DEBUG*/
  __os_print ("-- verify_redirection: ");
  __os_print (redir);
  __os_print ("\r\n");
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
  while (1)
    {
      /* Look for redirection operator `redirection_type'.  */
      while (*cmdline && *cmdline != redirection_type)
        cmdline++;

      if (*cmdline == '\0')
        /* Operator `redirection_type' doesn't exist.  */
    	return NULL;
      else
        {
	  /* Cope with "2> foobar" */
	  while (isdigit (cmdline[-1]))
	    cmdline --;

          if (verify_redirection (cmdline))
            return cmdline;
          /* Skip a character otherwise we loop on <foo$bar>.  */
          cmdline ++;
        }
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
  while (*cli)
    {
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
		   3. > filename.  */

	      /* Just incrementing cli by one here converts a <> filename
		 into a > filename.  We're not interested in the re-direction
		 stuff anyway.  */
	      while (*cli && isdigit (*cli))
		cli++;

	      if (cli[0] == '<' && cli[1] == '>')
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
		cli++;
	    }
	  else if (*cli == '\'')
	    {
	      /* The argument is contained within single quotes.  */
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
		cli++;
	    }
	  else
	    *p++ = *cli++;
	}

      *p = '\0';
      /* We have now reached a space delimiter.  Add the argument to
         the argv list, if one was actually made.  */
      if (p != temp)
        {
          argc ++;
          argv = (char **) realloc (argv, (argc + 1) * sizeof (char *));
          argv[argc - 1] = strdup (temp);
        }
  }

  /* Set the last item to NULL */
  argv[argc] = NULL;

  free (temp);
  process->argc = argc;
  process->argv = argv;

  return argc;
}
