/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fgets.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgets.c,v 1.3 2003/04/13 16:21:02 alex Exp $";
#endif

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
