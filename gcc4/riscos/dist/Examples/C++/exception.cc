/* Simple demonstration of exception handling.  */

#include <stdio.h>

void foo (void)
{
  printf ("in foo\n");
  throw ("hello world");
  printf ("in foo shouldn't be here\n");
}

int main (void)
{
  try {
    printf ("trying foo\n");
    foo ();
    printf ("in main shouldn't be here\n");
  }

  catch (const char *str)
    {
      printf ("caught exception: %s\n", str);
    }

  printf ("main. exiting\n");
  return 0;
}
