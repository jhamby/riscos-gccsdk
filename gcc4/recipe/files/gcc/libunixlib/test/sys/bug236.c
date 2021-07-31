/* forks and execs 'test236' with redirected stdin as 'file_in', stdout as
   'file_out' and stderr as 'file_err'.
   Expected output is (when no 'file_in' is present):

     -1 3 4
     1
     2

   And created files 'file_out' containing "test: stdout" and 'file_err'
   containing "test: stderr"
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		// open
#include <unistd.h>		// close
#include <sys/wait.h>		// wait WEXITSTATUS

static int
redirect (char *const cmd[], int file_in, int file_out, int file_err)
{
  pid_t childpid;		// Child's pid.
  int retval;			// Child process return code.
  int status;			// Parent process return code.

  childpid = fork ();		// Create new process.
  if (childpid == 0)
    {				// Child process.
      if (file_in >= 0)
	dup2 (file_in, STDIN_FILENO);	// Redirect standard input
      if (file_out >= 0)
	dup2 (file_out, STDOUT_FILENO);	// Redirect standard output
      if (file_err >= 0)
	dup2 (file_err, STDERR_FILENO);	// Redirect standard error
      retval = execvp (cmd[0], cmd);	// execvp() does not return on success, only on failure.
      return retval;
    }
  else if (childpid > 0)
    {				// Parent process.
      wait (&status);		// Wait for child to exit, and store its status.
      return WEXITSTATUS (status);
    }
  else
    {				// fork() returns -1 on failure.
      fprintf (stderr, "fork() error\n");
      exit (1);
    }
}

int
main (void)
{
  int file_in = open ("file_in", O_RDONLY);
  int file_out = open ("file_out", O_WRONLY | O_CREAT);
  int file_err = open ("file_err", O_WRONLY | O_CREAT);
  printf ("%i %i %i\n", file_in, file_out, file_err);

  printf ("1\n");

  char *cmd[] = { "test236", 0 };
  if (redirect (cmd, file_in, file_out, file_err) != 0)
    printf ("system error\n");

  close (file_in);
  close (file_out);
  close (file_err);

  printf ("2\n");

  return 0;
}
