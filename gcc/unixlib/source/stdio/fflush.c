/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fflush.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fflush.c,v 1.3 2003/04/13 16:21:02 alex Exp $";
#endif

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

int
fflush (FILE *stream)
{
  PTHREAD_UNSAFE

  if (stream == NULL)
    {
      int lossage = 0;

      /* Flush all streams.  */
      stream = __iob_head;
      while (stream)
        {
	  if (__validfp (stream) && stream->__mode.__write)
	    lossage |= __flsbuf (EOF, stream);
	  stream = stream->next;
	}
      return lossage ? EOF : 0;
    }

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return EOF;
    }

  if (!stream->__mode.__write)
    return 0;

  return __flsbuf (EOF, stream);
}
