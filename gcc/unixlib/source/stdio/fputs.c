/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fputs.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fputs.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <errno.h>

__STDIOLIB__

int
fputs (const char *s, FILE * stream)
{
  size_t length;

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
  if (fwrite ((void *) s, 1, length, stream) != length)
    return EOF;

  return 0;
}
