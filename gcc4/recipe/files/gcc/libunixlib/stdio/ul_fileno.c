/* fileno ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>

#include <internal/unix.h>

#undef	fileno

int
fileno (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    return __set_errno (EINVAL);

  return stream->fd;
}
