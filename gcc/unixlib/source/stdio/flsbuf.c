/* __flsbuf ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#ifdef DEBUG
#  include <unixlib/os.h>
#endif

__STDIOLIB__

/* __flsbuf(EOF, f) flushes output without adding c */

int
__flsbuf (int c, FILE *stream)
{
  unsigned char *buffer;

  PTHREAD_UNSAFE

  if (ferror (stream))
    return EOF;

#ifdef DEBUG
  __os_print ("__flsbuf("); __os_prdec (stream->fd); __os_print ("): ");
#endif

  if (stream->__pushedback)
    {
      /* Discard the char pushed back by ungetc.  */
      stream->i_cnt = stream->__pushedi_cnt;
      stream->__pushedback = 0;
    }

  if (stream->o_base != NULL)
    {
      int to_write, written;

      to_write = stream->o_ptr - stream->o_base;
#ifdef DEBUG
      __os_print ("to_write="); __os_prdec (to_write); __os_print ("\r\n");
#endif
      /* Skip write if 0 characters to write. Keeps perl happy, consistent
         with BSD.  */
      written = to_write ? write (stream->fd, stream->o_base, to_write) : 0;
      if (written < to_write)
        {
          stream->__error = 1;
          return EOF;
        }
      stream->__offset += to_write;
      stream->o_cnt = stream->__bufsize;
      stream->o_ptr = stream->o_base;
    }
#ifdef DEBUG
  else
    {
      __os_nl ();
    }
#endif

  if (c == EOF)
    return 0;

  /* Write out the last character.  */
  buffer = (unsigned char *)&c;
  if (write (stream->fd, buffer, 1) < 1)
    {
      stream->__error = 1;
      return EOF;
    }
  stream->__offset ++;

  return c;
}
