/* printf interface functions.
   Copyright (c) 2005-2011 UnixLib Developers.  */

#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <wchar.h>
#include <pthread.h>

int
__vsnprintf (char *buf, size_t limit, const char *format, va_list ap)
{
  const size_t ol = limit;

  if (limit != 0)
    limit --;
  if (limit > INT_MAX)
    limit = INT_MAX;

  /* Create a buffered FILE object and point the buffer to the user's
     buffer area.  Stdio should handle reaching the buffer limits.  */
  FILE f[1];
  f->o_ptr = f->o_base = (unsigned char *) buf;
  f->o_cnt = f->__bufsize = limit;

  /* Buffer needs to be write only.  */
  f->__mode.__bits.__read = f->__mode.__bits.__append = 0;
  f->__mode.__bits.__write = 1;

  f->__linebuf = 0;
  f->__eof = f->__error = 0;
  f->__offset = 0;

  /* This file descriptor value is specially interpreted by 'fwrite'.  */
  f->fd = -1;

  f->__magic = _IOMAGIC;

  /* We use the non-multithreaded printf function here as we can
     already guarantee exclusivity to the FILE structure.  */
  int ret = __vfprintf (f, format, ap);
  if (ol != 0)
    *f->o_ptr = '\0';

  return ret;
}
weak_alias (__vsnprintf, vsnprintf)

int
__vsprintf(char *buf, const char *format, va_list ap)
{
  return vsnprintf (buf, INT_MAX, format, ap);
}
weak_alias (__vsprintf, vsprintf)

int
__snprintf(char *buf, size_t limit, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  int r = vsnprintf (buf, limit, format, ap);
  va_end (ap);

  return r;
}
weak_alias (__snprintf, snprintf)

int
__sprintf (char *buf, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  int r = vsnprintf (buf, INT_MAX, format, ap);
  va_end (ap);

  return r;
}
weak_alias (__sprintf, sprintf)

int
__vprintf (const char *format, va_list ap)
{
  return vfprintf (stdout, format, ap);
}
weak_alias (__vprintf, vprintf)

int
__fprintf (FILE * stream, const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  int r = vfprintf (stream, format, ap);
  va_end (ap);

  return r;
}
weak_alias (__fprintf, fprintf)

int
__printf (const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  int r = vfprintf (stdout, format, ap);
  va_end (ap);

  return r;
}
weak_alias (__printf, printf)
