/* UnixLib fputc(), fputc_unlocked(), putc() and putc_unlocked() implementation.
   Copyright 2002-2011 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

int
fputc (int c, FILE *stream)
{
  PTHREAD_UNSAFE

  /* Do not yet check on stream->__mode.__bits.__write yet, this is done
     at __flsbuf time.  */
  if (!__validfp (stream))
    {
      __set_errno (EINVAL);
      return EOF;
    }

  c &= 0xFF;

  /* If stream is line buffered, then always flush when we receive a
     newline.  */
  if (stream->__linebuf)
    {
      if (c == '\n')
	return __flsbuf (c, stream);
      /* Mark there is at least one line buffered stdio stream which has an
	 unflushed buffer so that we can force flush it when reading from a
	 tty attached stream.  */
      __ul_global.fls_lbstm_on_rd = 1;
    }

  /* Add character to buffer.  If buffer fills, then flush the stream.  */
  if (--stream->o_cnt > 0)
    {
      *stream->o_ptr++ = c;
      return c;
    }

  return __flsbuf (c, stream);
}
strong_alias (fputc, putc)
strong_alias (fputc, fputc_unlocked)
strong_alias (fputc, putc_unlocked)

