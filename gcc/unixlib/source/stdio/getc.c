/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/getc,v $
 * $Date: 1997/10/09 20:00:34 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getc,v 1.7 1997/10/09 20:00:34 unixlib Exp $";
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
