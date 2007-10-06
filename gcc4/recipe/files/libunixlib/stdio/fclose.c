/* UnixLib fclose() implementation.
   Copyright 2001-2007 UnixLib Developers.  */

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <unixlib/unix.h>

#ifdef DEBUG
#include <unixlib/os.h>
#endif

int
fclose (FILE * stream)
{
  int status;

  PTHREAD_UNSAFE

  if (stream == NULL)
    {
      FILE *f;

      /* Close all streams.  */
      f = __iob_head;
      while (f)
        {
          if (__validfp (f))
            fclose (f);
          f = f->next;
        }
      return 0;
    }

  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

#ifdef DEBUG
  __os_print ("fclose("); __os_prdec (stream->fd); __os_print ("): ");
#endif

  /* Only flush writable streams.  */
  if (stream->__mode.__bits.__write && __flsbuf (EOF, stream) == EOF)
    {
#ifdef DEBUG
      __os_print ("EOF\r\n");
#endif
      return EOF;
    }

  /* Free the buffer's storage.  */
  if (!stream->__iuserbuf)
    free (stream->i_base);
  if (!stream->__ouserbuf)
    free (stream->o_base);

  /* Close the file descriptor.  */
  status = 0;
  status = close (stream->fd);

  /* Invalidate the stream, ready for re-use.  */
  __invalidate (stream);

  return (status < 0) ? EOF : 0;
}
