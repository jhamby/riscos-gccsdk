/* ferror ()
 * Copyright (c) 2000-2013 UnixLib Developers
 */

#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

#undef	ferror

/* Return non-zero if STREAM has its error indicator set.  */
int
ferror (FILE *stream)
{
  PTHREAD_UNSAFE

  /* An invalid stream may not set errno.  */
  return __validfp (stream) && stream->__error != 0;
}
