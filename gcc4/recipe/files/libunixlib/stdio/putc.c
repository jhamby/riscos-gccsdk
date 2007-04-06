/* UnixLib fputc(), fputc_unlocked(), putc() and putchar() implementation.
   Copyright 2002-2007 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

int
fputc (int c, FILE *stream)
{
  PTHREAD_UNSAFE

  if (! __validfp (stream) || ! stream->__mode.__bits.__write)
    {
      errno = EINVAL;
      return EOF;
    }

  /* If stream is line buffered, then always flush when we receive a
     newline.  */
  if (stream->__linebuf && c == '\n')
    return __flsbuf (c, stream);

  /* Add character to buffer.  If buffer fills, then flush the stream.  */
  if (--stream->o_cnt > 0)
    {
      /* I think that fputc should preserve the full 32-bits of `c',
	 even though only 8-bits are written out to the stream.  */
      *stream->o_ptr++ = c;
      return c;
    }
  else
    return __flsbuf (c, stream);
}

int
fputc_unlocked(int c, FILE *stream)
{
  return putc_unlocked(c, stream);
}

int
putc (int c, FILE *stream)
{
  return fputc (c, stream);
}

int
putchar (int c)
{
  return fputc (c, stdout);
}
