/* feof ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <unixlib/unix.h>

#undef	feof

/* Return non-zero if STREAM has its EOF indicator set.  */
int
feof (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    return __set_errno (EINVAL);

  return stream->__eof;
}
