/* Compile with cc -apcs 3/reent/fpr */
/* You can't actually run this, since it would require a printf that
   accepted arguments in the fp registers. */

#include <stdio.h>
#include <stdarg.h>

typedef struct int64 {
  int lo;
  int hi;
} int64;

void cool (int a, ...)
{
  va_list ap;
  int i;
  double d;
  va_start (ap, a);
  i = va_arg (ap, int);
  printf ("%d ", i);
  i = va_arg (ap, int);
  printf ("%d ", i);
  d = va_arg (ap, double);
  printf ("%f ", d);
  va_end (ap);
}


int main (void)
{
  int a = 1, b = 2, e = 3, h = 5, j = 6;
  double c = 1.0, d = 2.0, f = 3.0, g = 4.0, i = 5.0;
  int64 x;

  x.lo = 10;
  x.hi = 20;
/*cool (1, 2, 1.0, 2.0, 3, int64, 3.0, 4.0, 5, 5.0, 6); */
  cool (a, b, c, d, e, x, f, g, h, i, j);

  cool (a, b, d, e, x, f, g, h, i, j);


}
