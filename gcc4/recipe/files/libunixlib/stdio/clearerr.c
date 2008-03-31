/* clearerr ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

/* Clear the error and EOF indicators of stream.  */
void
clearerr (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return;
    }

  stream->__error = 0;
  stream->__eof = 0;
}
weak_alias (clearerr, clearerr_unlocked)

