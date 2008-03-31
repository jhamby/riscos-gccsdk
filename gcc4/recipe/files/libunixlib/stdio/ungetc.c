/* UnixLib ungetc() implementation.
   Copyright 2000-2008 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

int
ungetc (int c, FILE * stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || !stream->__mode.__bits.__read)
    {
      __set_errno (EINVAL);
      return EOF;
    }

  if (c == EOF)
    return EOF;

  if (stream->__pushedback)
    return EOF; /* A char has already been pushed back.  */

  stream->__pushedchar = c;
  /* Tell __filbuf we've pushed back a char. Keep a copy
     of i_cnt which will be restored by __filbuf.  */
  stream->__pushedback = 1;
  stream->__pushedi_cnt = stream->i_cnt;
  /* Make the next getc call __filbuf.  It will return 'c'.  */
  stream->i_cnt = 0;
  /* There's another char to read, so we're no longer at EOF.  */
  stream->__eof = 0;

  return c;
}
