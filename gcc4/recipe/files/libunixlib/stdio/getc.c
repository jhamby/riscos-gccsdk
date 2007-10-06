/* UnixLib fgetc(), getc() and getchar() implementation.
   Copyright 2000-2007 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unixlib/unix.h>

int
fgetc (FILE * stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || !stream->__mode.__bits.__read)
    return __set_errno (EINVAL);

  return getc_unlocked (stream);
}

#undef getc
#undef getchar

int
getc (FILE * stream)
{
  return fgetc (stream);
}

int
getchar (void)
{
  return fgetc (stdin);
}
