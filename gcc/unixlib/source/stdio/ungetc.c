/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/ungetc,v $
 * $Date: 1997/10/09 20:00:38 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ungetc,v 1.7 1997/10/09 20:00:38 unixlib Exp $";
#endif

#include <stdio.h>
#include <errno.h>

__STDIOLIB__

int
ungetc (int c, FILE * stream)
{
  if (!__validfp (stream) || !stream->__mode.__read)
    {
      errno = EINVAL;
      return EOF;
    }

  if (c == EOF)
    return EOF;

  if (stream->__pushedback)
    return EOF; /* A char has already been pushed back.  */

  stream->__pushedchar = c;
  /* Tell __filbuf we've pushed back a char. Keep a copy
     of i_cnt which will be restored by __filbuf.  */
  stream->__pushedback = 1;
  stream->__pushedi_cnt = stream->i_cnt;
  /* Make the next getc call __filbuf.  It will return 'c'.  */
  stream->i_cnt = 0;
  /* There's another char to read, so we're no longer at EOF.  */
  stream->__eof = 0;

  return c;
}
