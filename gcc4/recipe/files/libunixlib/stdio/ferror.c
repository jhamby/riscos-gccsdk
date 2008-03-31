/* ferror ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

#undef	ferror

/* Return non-zero if STREAM has its error indicator set.  */
int
ferror (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    return __set_errno (EINVAL);

  return stream->__error;
}
