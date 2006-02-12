/* printf interface functions.
   Copyright (c) 2005-2006 UnixLib Developers.  */

#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <wchar.h>
#include <pthread.h>



int vsnprintf (char *buf, size_t limit, const char *format, va_list ap)
{
  FILE f[1];
  int ret;
  size_t ol = limit;

  if (limit != 0)
    limit --;
  if (limit > INT_MAX)
    limit = INT_MAX;

  /* Create a buffered FILE object and point the buffer to the user's
     buffer area.  Stdio should handle reaching the buffer limits.  */
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
  ret = __vfprintf (f, format, ap);
  if (ol > 0)
    *f->o_ptr = '\0';

  return ret;
}


int vsprintf(char *buf, const char *format, va_list ap)
{
  return vsnprintf (buf, INT_MAX, format, ap);
}


int snprintf(char *buf, size_t limit, const char *format, ...)
{
  va_list ap;
  int r;

  va_start (ap, format);
  r = vsnprintf (buf, limit, format, ap);
  va_end (ap);

  return r;
}


int sprintf (char *buf, const char *format, ...)
{
  va_list ap;
  int r;

  va_start (ap, format);
  r = vsnprintf (buf, INT_MAX, format, ap);
  va_end (ap);

  return r;
}

int vprintf (const char *format, va_list ap)
{
  return vfprintf (stdout, format, ap);
}

int fprintf (FILE * stream, const char *format, ...)
{
  va_list ap;
  int r;

  va_start (ap, format);
  r = vfprintf (stream, format, ap);
  va_end (ap);

  return r;
}

int printf (const char *format, ...)
{
  va_list ap;
  int r;

  va_start (ap, format);
  r = vfprintf (stdout, format, ap);
  va_end (ap);

  return r;
}

/* weak_alias (__vsnprintf, vsnprintf)
   weak_alias (__vsprintf, vsprintf)
   weak_alias (__snprintf, snprintf)
   weak_alias (__sprintf, sprintf)
   weak_alias (__vprintf, vprintf)
   weak_alias (__fprintf, fprintf)
   weak_alias (__printf, printf) */
