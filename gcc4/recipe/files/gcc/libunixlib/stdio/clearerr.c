/* clearerr ()
 * Copyright (c) 2000-2013 UnixLib Developers
 */

#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

/* Clear the error and EOF indicators of stream.  */
void
clearerr (FILE *stream)
{
  PTHREAD_UNSAFE

  /* An invalid stream may not set errno.  */
  if (!__validfp (stream))
    return;

  stream->__eof = stream->__error = 0;
}
weak_alias (clearerr, clearerr_unlocked)

