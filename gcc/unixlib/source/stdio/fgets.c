/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/fgets,v $
 * $Date: 1997/10/09 20:00:33 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgets,v 1.7 1997/10/09 20:00:33 unixlib Exp $";
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
