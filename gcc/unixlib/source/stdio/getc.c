/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/getc.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getc.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

#include <stdio.h>
#include <errno.h>

__STDIOLIB__

int
fgetc (FILE * stream)
{
  if (!__validfp (stream) || !stream->__mode.__read)
    {
      errno = EINVAL;
      return -1;
    }
  return getc (stream);
}

#undef getc
#undef getchar

int
getc (FILE * stream)
{
  return fgetc (stream);
}

int
getchar (void)
{
  return fgetc (stdin);
}
