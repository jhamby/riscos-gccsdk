#include <stdio.h>


int main (void)
{
  try
  {
    throw 1;
  }
  catch (int x)
  {
    printf ("caught %d\n", x);
  }

  return 0;
}
