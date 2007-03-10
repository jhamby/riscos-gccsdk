/* fileno ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

#undef	fileno

int fileno (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return -1;
    }

  return stream->fd;
}
