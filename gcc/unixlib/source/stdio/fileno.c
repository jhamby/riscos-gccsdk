/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fileno.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fileno.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
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
