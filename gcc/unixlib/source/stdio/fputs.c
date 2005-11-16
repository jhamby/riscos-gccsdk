/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fputs.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

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
