#include <stdio.h>

int static_array[256];
int *dynamic_array;

void zzz (int *z, int *x)
{
  __builtin_memcpy (z, x, 16);
  printf ("%s %s\n", z, x);
}

void fubar (void)
{
  int local_static_array[256];
  int *local_dynamic_array;
  local_static_array[0] = 15;
  local_dynamic_array[0] = 24;
  zzz (local_static_array, local_dynamic_array);
}

int main (void)
{
  static_array[0] = 15;
  dynamic_array[0] = 24;
  fubar ();
  return 0;
}
