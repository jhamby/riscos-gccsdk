/* UnixLib ftell() implementation.
   Copyright 2001-2011 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

long
ftell (FILE *stream)
{
  __off_t pos;
  if (fgetpos (stream, &pos) == -1)
    return -1L;

#ifdef DEBUG
  debug_printf ("-- ftell(): pos %u\n", pos);
#endif
  if (pos > (__off_t)LONG_MAX)
    return -1L;

  return (long)pos;
}

