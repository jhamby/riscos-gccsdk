#include <stdarg.h>

void AssignPath (const int N, ...)
{
  va_list pvar;
  int va1;

  va_start (pvar, N);
  va1 = va_arg (pvar, int);
  va_end (pvar);
}
