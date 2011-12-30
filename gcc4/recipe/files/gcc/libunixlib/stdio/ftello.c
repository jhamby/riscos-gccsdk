/* UnixLib ftello(), ftello64() implementation.
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

__off_t
ftello (FILE *stream)
{
  __off_t pos;
  if (fgetpos (stream, &pos) == -1)
    return (__off_t)-1;

  return pos;
}

#if __UNIXLIB_LFS64_SUPPORT
#  error "64-bit LFS support missing."
#else
__off64_t
ftello64 (FILE *stream)
{
  return ftello (stream);
}
#endif

