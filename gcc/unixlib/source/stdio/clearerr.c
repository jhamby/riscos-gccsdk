/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/clearerr,v $
 * $Date: 1997/10/09 20:00:32 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: clearerr,v 1.4 1997/10/09 20:00:32 unixlib Exp $";
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
