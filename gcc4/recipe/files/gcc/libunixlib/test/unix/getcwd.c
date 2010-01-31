/* getcwd.c - test the getcwd functions.  */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void dir (const char *directory)
{
  char buffer[512];
  char *s;

  printf ("-- chdir  '%s'\n", directory);
  chdir (directory);
  s = getcwd (buffer, sizeof (buffer) - 1);
  if (s == NULL)
    {
      printf ("-- getcwd returned NULL: %s\n", strerror (errno));
      return;
    }
  printf ("-- getcwd '%s'\n", buffer);
}

int main (void)
{
  dir ("$");
  dir ("$.arm-riscos-aof");
  dir ("$.arm-riscos-aof.gcc-2_95_2.gcc.cp");
  dir ("$.gcc-2_95_2.gcc");
  return 0;
}



