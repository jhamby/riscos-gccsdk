/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/feof.c,v $
 * $Date: 2003/04/13 16:21:01 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

#undef	feof

/* Return non-zero if STREAM has its EOF indicator set.  */
int
feof (FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return -1;
    }

  return stream->__eof;
}
