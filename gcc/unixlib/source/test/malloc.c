/* Test malloc-free many times.  */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[], char *env[])
{
  int *buf[256];
  int x, y, z;

  for (x = 0; x < 400000; x++)
    {
      z = rand () % 255;
      for (y = 0; y < z; y++)
        {
          buf[y] = (int *) malloc (rand () % 4096);
          if (buf[y] == NULL)
            printf ("-- failed: x = %d, y = %d\n", x, y);
        }
      for (y = 0; y < z; y++)
        free (buf[y]);
    }

  return 0;
}
