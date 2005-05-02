/* Test vfork-and-exit many times.  */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[], char *env[])
{
  int x = 0, status;

   for (x = 0; x < 500; x++)
    {
      pid_t pid = fork ();
      if (pid == (pid_t) 0)
        {
          printf ("-- child process: %d (pid = %u)\n", x, pid);
          _exit (0);
        }
      else if (pid < (pid_t) 0)
        printf ("-- fork failed: %d\n", x);
      else
        {
          printf ("-- parent process: %d\n", x);
          waitpid (pid, &status, 0);
	  printf ("-- waitpid: status=%d\n", status);
        }
    }

  return 0;
}
