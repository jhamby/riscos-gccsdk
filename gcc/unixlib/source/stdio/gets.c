/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/gets.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: gets.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
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
