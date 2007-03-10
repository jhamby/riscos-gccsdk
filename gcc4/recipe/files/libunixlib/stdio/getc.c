/* UnixLib fgetc(), getc() and getchar() implementation.
   Copyright 2000-2006 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

int
fgetc (FILE * stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || !stream->__mode.__bits.__read)
    {
      errno = EINVAL;
      return -1;
    }
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
