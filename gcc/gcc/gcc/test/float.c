#include <stdio.h>

int main (void)
{
  float a = -0.0;
  /* float b = 0.0; */

  /* *(int *)&a ^= 0x80000000; */
  if (a < (float)0.0)
    printf ("less\n");
  printf ("%f\n", a);

  return 0;
}
