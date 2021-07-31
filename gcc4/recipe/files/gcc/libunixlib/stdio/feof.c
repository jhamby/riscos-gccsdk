/* feof ()
 * Copyright (c) 2000-2013 UnixLib Developers
 */

#include <stdio.h>

#include <pthread.h>
#include <internal/unix.h>

#undef	feof

/* Return non-zero if STREAM has its EOF indicator set.  */
int
feof (FILE *stream)
{
  PTHREAD_UNSAFE

  /* An invalid stream may not set errno.  */
  return __validfp (stream) && stream->__eof != 0;
}
