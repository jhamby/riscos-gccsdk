/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/fputs,v $
 * $Date: 1997/10/09 20:00:34 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fputs,v 1.6 1997/10/09 20:00:34 unixlib Exp $";
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
