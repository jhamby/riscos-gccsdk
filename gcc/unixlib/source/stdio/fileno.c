/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/fileno,v $
 * $Date: 1997/10/09 20:00:33 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fileno,v 1.4 1997/10/09 20:00:33 unixlib Exp $";
#endif

#include <errno.h>
#include <stdio.h>

#undef	fileno

int fileno (FILE *stream)
{
  if (!__validfp (stream))
    {
      errno = EINVAL;
      return -1;
    }
  return stream->fd;
}
