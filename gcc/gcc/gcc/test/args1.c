/* Ackermann function */
#include <stdarg.h>

#include <stdio.h>

void foo (double a, int e, ...);

void cool (int a, int b, int c, int d)
{
  printf ("cool: %d %d %d %d\n", a, b, c, d);
}

void smart (double a, double b, double c, double d)
{
  printf ("smart: %d %d %d %d\n", (int)a, (int)b, (int)c, (int)d);
}

void bar (double a, double b, double c, double d)
{
  printf ("bar: %d %d %d %d\n", (int)a, (int)b, (int)c, (int)d);
  smart (d, c, a, b);
}


void foo (double a, int e, ...)
{
  va_list ap;
  double b, c,d;

  va_start(ap, e);
/*  a = va_arg (ap, double); */
  b = va_arg (ap, double);
  c = va_arg (ap, double);
  d = va_arg (ap, double);
  smart (a, b, c, d);
  cool (e, va_arg (ap, int), va_arg (ap, int), va_arg (ap, int));
  smart (va_arg (ap, double), va_arg (ap, double),
  	 va_arg (ap, double), va_arg (ap, double));
  smart (va_arg (ap, double), va_arg (ap, double), 0, 0);

  va_end(ap);
}

void funky (int a, int b, double c, double d, int e,
     	    int f, double g, double h, int i, double j, int k)
{
  printf ("funky: a = %d, b = %d, c = %.2f", a, b, c);
  printf (", d = %.2f, e = %d, f = %d", d, e, f);
  printf (", g = %.2f, h = %.2f, i = %d", g, h, i);
  printf (", j = %.2f, k = %d\n", j, k);
}

void funky1 (int a, int b, double c, double d, ...)
{
  va_list ap;
  va_start (ap, d);
  funky (a, b, c, d, va_arg (ap, int), va_arg (ap, int), va_arg (ap, double),
  	 va_arg (ap, double), va_arg (ap, int), va_arg (ap, double), va_arg (ap, int));
  va_end (ap);
}

void temp (int a, double b, double c)
{
  printf ("temp: a = %d, b = %f, c = %f\n", a, b, c);
}

int main(void)
{
  printf ("calling funky:\n");
  funky (1, 2, 1.0, 2.0, 3, 100, 3.0, 4.0, 5, 5.0, 6);
  printf ("calling funky1:\n");
  funky1 (1, 2, 1.0, 2.0, 3, 100, 3.0, 4.0, 5, 5.0, 6);
  printf ("calling foo:\n");
  foo (1.0, 3, 2.0, 4.0, 5.0, 10, 11, 12, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
  bar (100.0, 200.0, 300.0, 400.0);
  temp (10, 50.0, 70.0);
  return 0;
}


