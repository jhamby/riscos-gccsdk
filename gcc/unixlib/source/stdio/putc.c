/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/putc.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putc.c,v 1.3 2002/02/14 15:56:36 admin Exp $";
#endif

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

int
fputc (int c, FILE *stream)
{
  PTHREAD_UNSAFE

  if (! __validfp (stream) || ! stream->__mode.__write)
    {
      errno = EINVAL;
      return EOF;
    }

  /* If stream is line buffered, then always flush when we receive a
     newline.  */
  if (stream->__linebuf && c == '\n')
    return __flsbuf (c, stream);

  /* Add character to buffer.  If buffer fills, then flush the stream.  */
  if (--stream->o_cnt > 0)
    {
      /* I think that fputc should preserve the full 32-bits of `c',
	 even though only 8-bits are written out to the stream.  */
      *stream->o_ptr++ = c;
      return c;
    }
  else
    return __flsbuf (c, stream);
}


int
putc (int c, FILE *stream)
{
  return fputc (c, stream);
}

int
putchar (int c)
{
  return fputc (c, stdout);
}
