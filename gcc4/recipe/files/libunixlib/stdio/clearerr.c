/* clearerr ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

/* Clear the error and EOF indicators of stream.  */
void clearerr (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return;
    }

  stream->__error = 0;
  stream->__eof = 0;
}
