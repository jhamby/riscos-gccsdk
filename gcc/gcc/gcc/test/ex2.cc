#include <stdio.h>

void zzz (void)
{
  throw 1;
}

int main (void)
{
  try
  {
    zzz ();
    /* throw 1; */
  }
  catch (int x)
  {
    printf ("caught %d\n", x);
  }

  return 0;
}
