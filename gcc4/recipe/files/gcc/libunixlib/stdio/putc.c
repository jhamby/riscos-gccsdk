/* UnixLib fputc(), fputc_unlocked(), putc(), putc_unlocked(). and putchar() implementation.
   Copyright 2002-2010 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

int
fputc (int c, FILE *stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || !stream->__mode.__bits.__write)
    {
      __set_errno (EINVAL);
      return EOF;
    }

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
      /* I think that fputc should preserve the full 32-bits of `c',
	 even though only 8-bits are written out to the stream.  */
      *stream->o_ptr++ = c;
      return c;
    }

  return __flsbuf (c, stream);
}
strong_alias (fputc, putc)

int
fputc_unlocked(int c, FILE *stream)
{
  return fputc (c, stream);
}
strong_alias (fputc_unlocked, putc_unlocked)

int
putchar (int c)
{
  return fputc (c, stdout);
}
