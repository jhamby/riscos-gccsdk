#include <stdio.h>

unsigned short cool[] = { 0x1234, 0x5678, 0x9347, 0x3482 };

int add (short a, short b)
{
  return (int) a + b;
}

int main (void)
{
  short a,b;
  int c;

  a = cool[1];
  b = cool[2];
  c = add (a, b);
  printf ("a = %d, b = %d, c = %d\n", a, b, c);
  return 0;
}
