/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/clearerr.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: clearerr.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

#include <errno.h>
#include <stdio.h>

/* Clear the error and EOF indicators of stream.  */
void clearerr (FILE *stream)
{
  if (!__validfp (stream))
    {
      errno = EINVAL;
      return;
    }
  stream->__error = 0;
  stream->__eof = 0;
}
