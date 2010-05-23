#include <stdio.h>
#include <stdlib.h>

void
recurse (int mask, int count, int level)
{
  printf ("%2d: recurse(%x,%d)\n", level, mask, count);
  if (!level)
    {
      printf ("%2d: leaving A\n", level);
      return;
    }
  if (!(mask & 1))
    {
      printf ("%2d: recursing\n", level);
      recurse (mask >> 1, count, level - 1);
      printf ("%2d: leaving B\n", level);
      return;
    }
  printf ("%2d: creating test[%d]\n", level, count);
  int test[count];
  if (level > 1)
    {
      printf ("%2d: recursing\n", level);
      recurse (mask >> 1, count, level - 1);
    }
  printf ("%2d: leaving C\n", level);
}

int
main (int argc, char **argv)
{
  for (int i = 0; i < (1<<8); ++i)
    recurse (i, 1, 7);
  return EXIT_SUCCESS;
}
