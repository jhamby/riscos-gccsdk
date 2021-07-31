/* system ()
 * Written by Nick Burrett, 6 October 1996.
 * Copyright 1996-2010 UnixLib Developers
 */

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#include <pthread.h>
#include <internal/unix.h>

/* Execute LINE as a shell command, returning its status.  */
int
system (const char *line)
{
  int status;
  pid_t pid;
  struct sigaction sa, intr, quit;
  sigset_t block, omask;

  PTHREAD_SAFE_CANCELLATION

  if (line == NULL)
    return 1;

  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  sigemptyset (&sa.sa_mask);

  /* Ignore signals SIGINT and SIGQUIT.  */

  if (sigaction (SIGINT, &sa, &intr))
    return -1;

  if (sigaction (SIGQUIT, &sa, &quit))
    {
      /* Bit of an error, restore SIGINT and preserve the errno.  */
      int save = errno;
      (void) sigaction (SIGINT, &intr, (struct sigaction *) NULL);
      return __set_errno (save);
    }

  /* Block SIGCHLD.  */
  sigemptyset (&block);
  sigaddset (&block, SIGCHLD);
  if (sigprocmask (SIG_BLOCK, &block, &omask))
    {
      /* Awkward error, so restore signal handlers and return.  */
      int save = errno;
      sigaction (SIGINT, &intr, (struct sigaction *) NULL);
      sigaction (SIGQUIT, &quit, (struct sigaction *) NULL);
      return __set_errno (save);
    }

  /* vfork the process.  */
  pid = vfork ();
  if (pid == (pid_t) 0)
    {
      /* Restore the signals.  */
      sigaction (SIGINT, &intr, (struct sigaction *) NULL);
      sigaction (SIGQUIT, &quit, (struct sigaction *) NULL);
      /* Could deliver a few pending signals here.  */
      sigprocmask (SIG_SETMASK, &omask, (sigset_t *) NULL);

      /* Execute the shell.  */
      const char *path;
      if (!(path = getenv ("SHELL")))
	{
	  if (*line == '*')
	    execl (line, 0);
	  else
	    execl ("*", "", line, 0);
	}
      else
	{
	  const char *shell = strrchr (path, '/');
	  if (shell)
	    shell++;
	  else
	    shell = path;
	  execl (path, shell, "-c", line, 0);
	}
      _exit (EXIT_FAILURE);
    }
  else if (pid < (pid_t) 0)
    /* The fork failed.  */
    status = -1;
  else if (waitpid (pid, &status, 0) != pid)
    status = -1;

  /* Got here on a vfork failure. So restore signals and the blocking
     mask then exit. */
  if ((sigaction (SIGINT, &intr, (struct sigaction *) NULL)
       | sigaction (SIGQUIT, &quit, (struct sigaction *) NULL)
       | sigprocmask (SIG_SETMASK, &omask, (sigset_t *) NULL)) != 0)
    return -1;

  return status;
}
