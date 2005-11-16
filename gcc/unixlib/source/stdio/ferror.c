/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/ferror.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

#undef	ferror

/* Return non-zero if STREAM has its error indicator set.  */
int
ferror (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return -1;
    }

  return stream->__error;
}
