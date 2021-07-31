/* Print to allocated string.
   Copyright (c) 2005-2008 UnixLib Developers.  */

#include <stdarg.h>
#include <stdio.h>
#include <sys/cdefs.h>

int
__asprintf (char **buf, const char *fmt, ...)
{
  va_list ap;
  int r;

  va_start (ap, fmt);
  r = vasprintf (buf, fmt, ap);
  va_end (ap);

  return r;
}
weak_alias (__asprintf, asprintf)
