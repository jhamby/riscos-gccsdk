#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>

char *alloc (int size)
{
  return malloc (size);
}

void bar (char *s, int b, int c)
{
  printf (s, "%d %d", b, c);
}

void foo (int a, int b, int c)
{
  char *s = alloca (a);

  bar (s, b, c);
}

int main (void)
{
  foo (20, 30, 40);
  return 0;
}
