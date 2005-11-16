/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/clearerr.c,v $
 * $Date: 2003/04/13 16:21:01 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
