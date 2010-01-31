/* UnixLib fclose() implementation.
   Copyright 2001-2008 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>
#include <internal/unix.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

int
fclose (FILE * stream)
{
  int status;

  PTHREAD_UNSAFE

  if (stream == NULL)
    {
      FILE *f;
      int status = 0;

      /* Close all streams.  */
      for (f = __iob_head; f != NULL; f = f->next)
        {
          if (__validfp (f))
            status |= fclose (f);
        }

      return status;
    }

  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

#ifdef DEBUG
  debug_printf ("fclose(%d): ", stream->fd);
#endif

  /* Only flush writable streams.  */
  if (stream->__mode.__bits.__write && __flsbuf (EOF, stream) == EOF)
    {
#ifdef DEBUG
      debug_printf ("EOF\n");
#endif
      return EOF;
    }

  /* Free the buffer's storage.  */
  if (!stream->__iuserbuf)
    free (stream->i_base);
  if (!stream->__ouserbuf)
    free (stream->o_base);

  /* Close the file descriptor.  */
  status = close (stream->fd);

  /* Invalidate the stream, ready for re-use.  */
  __invalidate (stream);

#ifdef DEBUG
  debug_printf ("status=%d\n", status);
#endif

  return (status < 0) ? EOF : 0;
}
