/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/gets,v $
 * $Date: 1997/10/09 20:00:35 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: gets,v 1.5 1997/10/09 20:00:35 unixlib Exp $";
#endif

#include <stdio.h>
#include <errno.h>

__STDIOLIB__

char *
gets (char *_s)
{
  FILE *stream = stdin;
  char *s = _s;
  int c = 0;

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
