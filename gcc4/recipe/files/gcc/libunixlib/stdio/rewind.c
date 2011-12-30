/* UnixLib rewind() implementation.
   Copyright 2001-2011 UnixLib Developers.  */

#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

void
rewind (FILE *stream)
{
  PTHREAD_UNSAFE

  /* Clear any file errors before and after the seek operation.  */
  clearerr (stream);
  fseek (stream, 0, SEEK_SET);
  clearerr (stream);
}
