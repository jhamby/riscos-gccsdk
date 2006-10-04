/* feof ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

#undef	feof

/* Return non-zero if STREAM has its EOF indicator set.  */
int
feof (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return -1;
    }

  return stream->__eof;
}
