#include <stdio.h>

int
main (int argc, char *argv[])
{
  fprintf (stdout, "test: stdout\n");
  fprintf (stderr, "test: stderr\n");
  return 0;
}
