/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/unix_orig.c,v $
 * $Date: 2000/07/15 14:52:45 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: unix_orig.c,v 1.1.1.1 2000/07/15 14:52:45 nick Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>

#include <sys/os.h>
#include <sys/unix.h>
#include <sys/syslib.h>
#include <sys/param.h>
#include <sys/swis.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>

#define DEBUG 1

#ifdef DEBUG
#include <sys/debug.h>
#endif

#ifndef __GNUC__
#undef attribute
#define attribute(x) /* ignore */
#endif

static void initialise_process_structure (struct proc *process);
static struct proc *create_process_structure (void);
static void initialise_unix_io (struct proc *process);
static void check_fd_redirection (const char *filename, int fd_to_replace);
static int get_fd_redirection (const char *redir);
static const char *find_terminator (const char *s);
static void get_io_redir (const char *cli);
static int verify_redirection (const char *redirection);
static const char *find_redirection_type (const char *command_line,
       	     	   			  char redirection_type);
static int convert_command_line (const char *command_line,
       	   	   	   	 int command_line_size);

static void __badr (void) __attribute__ ((__noreturn__));

/* Initialize the signal code.  If declared in sys/syslib.h
   then we'd have to drag in a lot of headers.  */
extern void __unixlib_signal_initialise (struct proc *__p);

/* Resource limit initialisation */
extern void __resource_initialise (struct proc *__p);

/* Globally used panic button.  */
void
__unixlib_fatal (const char *message)
{
  if (message == NULL)
    message = sys_errlist[errno];
  os_nl ();
  os_print (message);
  os_nl ();
  _exit (1);
}

static void
__badr (void)
{
  __unixlib_fatal ("Bad redirection");
}

#ifndef __GNUC__
/* Macros to simulate 'noreturn' functions.  */
#define __badr() do { __badr (); return; } while (0)
#define __unixlib_fatal(msg) do { __unixlib_fatal (msg); return; } while (0)
#endif

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


/* Initialise the UnixLib world.  */
void __unixinit (void)
{
  int i, cli_size = 0, newproc = 0;
  char *cli = NULL;

  __u = (struct proc *) __intenv ("UnixLib$env");
  if (__u != NULL)
    __remenv_from_os ("UnixLib$env");

create_process:
  if (__u == NULL || __u->__magic != _PROCMAGIC || __u->argb == '\0')
    {
#ifdef DEBUG
      os_print ("-- new process\r\n");
#endif
      /* Flag that we are a new process.  */
      newproc = 1;
      /* Create a process structure if no parent exists or the one
         pointed to by UnixLib$env is invalid.  */
      __u = create_process_structure ();
      if (__u == NULL)
        __unixlib_fatal ("cannot allocate memory for process structure");
      initialise_process_structure (__u);
      __resource_initialise (__u);
      __unixlib_signal_initialise (__u);
      /* Define and initialise the Unix I/O.  */
      initialise_unix_io (__u);
    }


  /* Get command line.  */
  if (! cli)
    {
      extern char *__cli;
      int __cli_size = strlen (__cli);
      int regs[10];

#ifdef DEBUG
      os_print ("-- getting cli: __cli = ");
      os_print (__cli); os_print ("\r\n");
#endif

      /* Get extra command line from DDEUtils.  */
      if (os_swi (DDEUtils_GetCLSize, regs))
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
	      os_swi (DDEUtils_GetCl, regs);
            }
        }

      /* Set command line length to zero otherwise the next process
      	 will also get it.  */
      regs[0] = 0;
      os_swi (DDEUtils_SetCLSize, regs);

      if (cli == NULL)
        __unixlib_fatal ("command line too long (not enough memory)");
    }

  /* If we are a child process, validate incoming command line
     against command line in process structure.  If they differ then
     assume the parent process structure is corrupt.  */
#ifdef DEBUG
  if (! newproc)
    {
      os_print ("-- child:  cli="); os_print (cli); os_print ("\r\n");
      os_print ("-- child: argb="); os_print (__u->argb); os_print ("\r\n");
    }
#endif
  if (! newproc && strcmp (cli, __u->argb))
    {
      __u = NULL;
      goto create_process;
    }

  /* Inherit environ from parent.  This is our copy, to do with as we
     like except for freeing.  */
  if (! newproc)
    environ = __u->envp;
  else
    {
      /* FIXME, copy environ from parent. Consider setting
         last_environ here if malloc'ing */
      environ = malloc (sizeof (char *));
      if (environ == NULL)
	__unixlib_fatal (NULL);
      *environ = NULL;
    }

  /* Find any environment variables.  */
  __runtime_features (cli);

  if (newproc)
    {
#ifdef DEBUG
      os_print ("-- create argv and argb: cli_size = ");
      os_prdec (cli_size); os_print ("\r\n");
#endif
      /* Allocate argv array for pointers into argv[] and single string for
         argv[] (i.e., all the arguments). The maximum number of arguments
     	 is (strlen(cli)+1)/2+1 and the maximum required string length is
     	 <= strlen(cli). */
      __u->argv = malloc (((cli_size >> 1) + 1) * sizeof (char *));
      if (__u->argv == NULL)
        __unixlib_fatal ("cannot allocate enough memory for command line");

      __u->argb = malloc (cli_size + 1);
      if (__u->argb == NULL)
	__unixlib_fatal ("cannot allocate enough memory for command line");

      /* Parse the command line, looking for I/O redirection.  */
      get_io_redir (cli);
      /* Convert the command line to a argv block.  */
      __u->argc = convert_command_line (cli, cli_size);
    }

  free (cli);

  i = __intenv ("Unix$uid");
  if (i != 0)
    __u->uid = __u->euid = i;

#ifdef DEBUG
  __debug ("__unixinit() (final)");
#endif
}

void
__unixexit (void)
{
  int i;
  struct __unixlib_fd *fd;

#if __FEATURE_ITIMERS
  /* Stop any interval timers that might be running.  */
  __stop_itimers ();
#endif

#ifdef DEBUG
  __debug ("__unixexit()");
#endif
/*  os_print ("__unixexit()\n\r");
   __display_redirection(); */

  fd = __u->fd;

  /* Close all file descriptors.  */
  if (fd)
    for (i = 0; i < MAXFD; i++)
      if (fd[i].__magic == _FDMAGIC)
	close (i);

#ifdef DEBUG
  __debug ("__unixexit() (close())");
#endif
}

int
__alloc_file_descriptor (void)
{
  struct __unixlib_fd *fd;
  int i;

  fd = __u->fd;

  /* Look for a spare file descriptor.  Suitably unallocated
     ones will have an invalid magic number.  */
  for (i = 0; i < MAXFD; i++)
    if (fd[i].__magic != _FDMAGIC)
      {
#ifdef DEBUG
        os_print ("__alloc_file_descriptor: found free descriptor ");
        os_prhex (i);
        os_nl ();
#endif
        return i;
      }

  /* Set errno to indicate the unavailability of file handles.  */
  return __set_errno (EMFILE);
}

#if __FEATURE_ITIMERS
void
__stop_itimers (void)
{
  struct itimerval new_timer;

  /* Interval timers are not implemented in task windows so we don't
     need to stop them.  */
  if (__taskwindow)
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
#endif

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
#if 0
  /* These don't need to be set because the process structure is initialised
     to zero in create_process_structure.  */
  process->pproc = 0;
  process->umask = 0;
  /* The priority of this process will currently be zero.  */
  process->ppri = 0;
  process->upri = 0;
  process->gpri = 0;
  /* We need to:
     1. set the resource usage for this child to zero.
     2. set the status for this child to zero.
     3. initialise the itimer structures (since no itimers are running).  */
  memset (&process->usage, 0, sizeof (struct rusage));
  memset (&process->status, 0, sizeof (struct __process));
#if __FEATURE_ITIMERS
  memset (&process->itimers, 0, sizeof (struct itimerval) * (__MAX_ITIMERS - 1));
#endif
#endif
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

#ifndef __TTY_STATIC_BUFS
  /* While not strictly necessary to allocate the del buffer here,
     it helps when we need to print a fatal message due to lack
     of memory during a later part of the initialisation of Unixlib.  */
  for (i = 0; i < MAXTTY; i++)
    {
      p->tty[i].del = malloc (_POSIX_PATH_MAX);
      if (p->tty[i].del == NULL)
        return NULL;
    }
#endif

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
  char *tty;

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
check_fd_redirection (const char *filename, int fd_to_replace)
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

#ifdef DEBUG
  os_print ("-- check_io_redir: redirecting fd ");
  os_prdec (fd); os_print ("\r\n");
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
  os_print ("-- check_io_redir: filename = '");
  os_print (fn); os_print ("'\r\n");
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
	 __unixlib_fatal (NULL);

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
  os_print ("-- get_io_redir: checking <\r\n");
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
  os_print ("-- get_io_redir: checking >\r\n");
#endif
  while ((p = find_redirection_type (p, '>')))
    {
      if (p[-1] != '<')
        {
          if (p[1] == '>')
            p++, check_io_redir (p, STDOUT_FILENO, mode);
          else
            check_io_redir (p, STDOUT_FILENO, mode | O_TRUNC);
        }
      p++;
    }
}

/* Verify that 'redirection' is actually pointing to a redirection
   operator. Return 0 if it is not, return 1 if it is.  */
static int
verify_redirection (const char *redir)
{
#if 0 /*def DEBUG*/
  os_print ("-- verify_redirection: ");
  os_print (redir);
  os_print ("\r\n");
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

  if ((redir[0] == '>' || redir[0] == '<')
      && (redir[1] == ' ' || isdigit (redir[-1])))
    return 1;

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

     An array index of -1 is safe here, becuase its impossible for
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
convert_command_line (const char *command_line, int command_line_size)
{
  int argc;
  char *argv_string;
  char **argv_pointers;
  const char *cli = command_line;

  argc = 0;
  argv_string = __u->argb;
  argv_pointers = __u->argv;

  while (*cli && argc < (command_line_size >> 1))
    {
      /* Skip any white space.  */
      while (isspace (*cli))
	cli++;

      /* Finish if we have reached the end of the command line.  */
      if (*cli == '\0')
	break;

      /* Mark the position in the argv string where the next argument is
	 about to go.  */
      argv_pointers[argc] = argv_string;

      /* We check for *cli != NULL here because of a command line like:
	    main 5>fred

	 verify_redirection will catch this and move cli onto the
	 terminating character (in this case, the null).  NULL is not
	 a space, so we'd start writing garbage into the argv array.  */
      while (*cli && !isspace (*cli))
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

	      /* By subtracting these, we nullify the effect of the
		 opposite operations at the end of the inner while
		 statement.  This is to prevent a null argv pointer.  */
	      argc--;
	      argv_string--;
	    }
	  else if (*cli == '\\')
	    {
	      /* Escape character.  */
	      if (*++cli != '\0')
		*argv_string++ = *cli++;
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
		      cli++;
		      if (*cli != '\0')
			*argv_string++ = *cli++;
		      else
			break;
		    }
		  else
		    *argv_string++ = *cli++;
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
		*argv_string++ = *cli++;
	      /* If we've finished on the single quote mark, then skip it.  */
	      if (*cli == '\'')
		cli++;
	    }
	  else
	    *argv_string++ = *cli++;
	}
      /* We've now found the space delimiter, so zero terminate the argv_string
	 and increment the number of arguments we've found.  */
      argc++;
      *argv_string++ = '\0';
    }
  /* The entire command line has now been processed.  Just terminate the
     argv list and that is all there is to it.  */
  argv_pointers[argc] = NULL;

  {
    /* Length of argb buffer.  */
    size_t length = argv_string - *argv_pointers;
    size_t *offset;
    char **argv;

    __u->clean_argv = malloc (sizeof (char *) * argc);
    if (__u->clean_argv == NULL)
      __unixlib_fatal (NULL);
    __u->clean_argb = malloc (length);
    if (__u->clean_argb == NULL)
      __unixlib_fatal (NULL);

    /* Copy command line arguments.  */
    memcpy (__u->clean_argb, __u->argb, length);

    /* Convert pointers into offsets, from end to start.  */
    offset = __u->clean_argv + argc;
    argv = __u->argv + argc;

#ifdef DEBUG
    os_print ("Lengths ");
    os_prhex (sizeof (char *) * argc);
    os_print (", ");
    os_prhex (length);
    os_print (", offset =");
    os_prhex ((int)offset);
    os_print ("\n\r");
#endif


    do {
#ifdef DEBUG
      os_print ("Copying ");
      os_prhex (*(argv - 1) - __u->argb);
      os_print (" '");
      os_print (*(argv-1));
      os_print ("'\n\r");
#endif
    } while ((*--offset = (*--argv - __u->argb)));
    /* argv[0] == argb, hence last offset is 0 and loop terminates.  */
    /* Store length of argb instead of this zero offset.  */
    *offset = length;

#ifdef DEBUG
    os_print ("offset =");
    os_prhex ((int)offset);
    os_print ("\n\r");
#endif
  }

  return argc;
}
