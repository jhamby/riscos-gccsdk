/* UnixLib fflush() implementation.
   Copyright 2000-2011 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

int
fflush (FILE *stream)
{
  PTHREAD_UNSAFE

  if (stream == NULL)
    {
      /* Flush all streams.  */
      int lossage = 0;
      for (stream = __iob_head; stream != NULL; stream = stream->next)
        {
	  if (__validfp (stream) && stream->__mode.__bits.__write)
	    lossage |= __flsbuf (EOF, stream);
	}

      /* When all our stdio streams have been flushed successfully, we don't
	 have to worry about pending data in our line buffered ones.  */
      if (!lossage)
	__ul_global.fls_lbstm_on_rd = 0;

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
strong_alias (fflush, fflush_unlocked)
