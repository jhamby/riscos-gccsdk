/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fgets.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

char *fgets (char *s, int n, FILE * stream)
{
  char *p = s;
  int c = 0;

  PTHREAD_UNSAFE

  if (!__validfp (stream) || s == NULL || n == 0)
    {
      errno = EINVAL;
      return NULL;
    }

  if (ferror (stream))
    return NULL;

  /* Repeat, obtaining characters until we hit a newline,
     or exceed our count, n.  */
  while (--n > 0 && (c = getc (stream)) != EOF)
    if ((*p++ = c) == '\n')
      break;

  if (c == EOF && (p == s || ferror (stream)))
    return NULL;

  *p = '\0';
  return s;
}
