/* fputs ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

int
fputs (const char *s, FILE * stream)
{
  size_t length;

  PTHREAD_UNSAFE

  /* Check file steam validity.  */
  if (!__validfp (stream))
    {
      errno = EINVAL;
      return EOF;
    }

  if (s == NULL)
    return 0;

  length = strlen (s);

  /* Fast case for a one character write operation.  */
  if (length == 1)
    return putc (*s, stream) == EOF ? EOF : 0;

  /* Use the buffered optimisations performed by fwrite().  */
  if (fwrite ((const void *) s, 1, length, stream) != length)
    return EOF;

  return 0;
}
