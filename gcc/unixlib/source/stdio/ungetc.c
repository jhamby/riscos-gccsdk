/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/ungetc.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ungetc.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

int
ungetc (int c, FILE * stream)
{
  PTHREAD_UNSAFE

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
