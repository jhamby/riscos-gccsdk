/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fgets.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgets.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

#include <stdio.h>
#include <errno.h>

__STDIOLIB__

char *
fgets (char *s, size_t n, FILE * stream)
{
  char *p = s;
  int c = 0;

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
