/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/clearerr.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: clearerr.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

/* Clear the error and EOF indicators of stream.  */
void clearerr (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return;
    }
  stream->__error = 0;
  stream->__eof = 0;
}
