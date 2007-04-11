/* Test vfork-exec-and-exit many times.  */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[], char *env[])
{
  int x, status;

  for (x = 0; x < 70; x++)
    {
      pid_t pid = vfork ();
      if (pid == (pid_t) 0)
        {
          printf ("-- child process: %d (pid = %u)\n", x, pid);
          if (execl ("*help", "", NULL))
            printf ("-- child process: execl failed: %s\n",
            	    strerror (errno));
          _exit (0);
        }
      else if (pid < (pid_t) 0)
        printf ("-- fork failed: %d\n", x);
      else
        {
          printf ("-- parent process: %d\n", x);
          waitpid (pid, &status, 0);
        }
    }

  return 0;
}
