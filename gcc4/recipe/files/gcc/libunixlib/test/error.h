#include <stdarg.h>
#include <stdio.h>

static inline void error (int status, int errnum, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  vfprintf (stdout, format, ap);
}
