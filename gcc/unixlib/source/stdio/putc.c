/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/putc.c,v $
 * $Date: 2000/07/15 14:52:32 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putc.c,v 1.1.1.1 2000/07/15 14:52:32 nick Exp $";
#endif

#include <stdio.h>
#include <errno.h>

__STDIOLIB__

int
fputc (int c, FILE * stream)
{
  if (!__validfp (stream) || !stream->__mode.__write)
    {
      errno = EINVAL;
      return EOF;
    }
  return putc (c, stream);
}

#undef putc
#undef putchar

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
