#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main (int argc, char *argv[], char *env[])
{
  int x, v;
  char *cmdline;

  cmdline = (char *) malloc (65536);
  sprintf (cmdline, "t2 make -C libio \"VERSION=2_95_2\" \"BOB=-march -mcpu -munixlib\"");

#if 1
  strcpy (cmdline, "t2 ");
  v = (rand () % 14) + 1;
  for (x = 3; x < 65500; x++)
    {
      cmdline[x] = 'A' + (rand () % 26);
      if (x % v == 0)
        {
          cmdline[x] = ' ';
          v = (rand () % 14) + 1;
        }
    }
  cmdline[x] = '\0';
#endif

  for (x = 0; x < 500; x++)
    {
      printf ("t1 -- about to call t2: iteration = %d\n", x);
  /* x = execvp ("help", argv); */
  /* printf ("x = %d, errno = %s\n", x, strerror (errno)); */
     if (system (cmdline))
       return 1;
     printf ("t1 -- t2 complete\n");
    }
  return 0;
}
