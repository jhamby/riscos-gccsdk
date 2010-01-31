/* fputs ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

int
fputs (const char *s, FILE * stream)
{
  size_t length;

  PTHREAD_UNSAFE

  /* Check file steam validity.  */
  if (!__validfp (stream))
    {
      __set_errno (EINVAL);
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
