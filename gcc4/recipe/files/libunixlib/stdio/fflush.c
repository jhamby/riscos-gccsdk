/* UnixLib fflush() implementation.
   Copyright 2000-2007 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unixlib/unix.h>

int
fflush (FILE *stream)
{
  PTHREAD_UNSAFE

  if (stream == NULL)
    {
      int lossage = 0;

      /* Flush all streams.  */
      for (stream = __iob_head; stream != NULL; stream = stream->next)
        {
	  if (__validfp (stream) && stream->__mode.__bits.__write)
	    lossage |= __flsbuf (EOF, stream);
	}

      return lossage ? EOF : 0;
    }

  if (!__validfp (stream))
    {
      __set_errno (EINVAL);
      return EOF;
    }

  if (!stream->__mode.__bits.__write)
    return 0;

  return __flsbuf (EOF, stream);
}
