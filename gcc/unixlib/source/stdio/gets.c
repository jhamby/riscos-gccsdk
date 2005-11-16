/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/gets.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

char *
gets (char *_s)
{
  FILE *stream = stdin;
  char *s = _s;
  int c = 0;

  PTHREAD_UNSAFE

  if (!__validfp (stream) || s == NULL)
    {
      errno = EINVAL;
      return NULL;
    }

  if (feof (stream) || ferror (stream))
    return NULL;

  while ((c = getc (stream)) != EOF)
    if (c == '\n')
      break;
    else
      *s++ = c;

  *s = '\0';

  /* Return NULL if we had an error, or if we got EOF before
     writing any chars.  */
  if (ferror (stream) || (feof (stream) && s == _s))
    return NULL;

  return _s;
}
