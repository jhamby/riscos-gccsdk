/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/putc.c,v $
 * $Date: 2002/01/15 13:21:19 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putc.c,v 1.2.2.1 2002/01/15 13:21:19 admin Exp $";
#endif

#include <stdio.h>
#include <errno.h>

__STDIOLIB__

int
fputc (int c, FILE *stream)
{
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
