/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/putc,v $
 * $Date: 1997/10/09 20:00:36 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putc,v 1.8 1997/10/09 20:00:36 unixlib Exp $";
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
