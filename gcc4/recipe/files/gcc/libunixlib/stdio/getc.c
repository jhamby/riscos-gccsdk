/* UnixLib fgetc(), fgetc_unlocked() and getc() implementation.
   Copyright 2000-2011 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

int
fgetc (FILE * stream)
{
  PTHREAD_UNSAFE

  /* Do not yet check on stream->__mode.__bits.__read yet, this is done
     at __filbuf/checkstream time.  */
  if (!__validfp (stream))
    {
      __set_errno (EINVAL);
      return EOF;
    }

  /* When we have possibly unflushed data in one of our line buffered streams
     and we're reading from a tty attached stream, flush all those streams.  */
  if (stream->__mode.__bits.__read
      && __ul_global.fls_lbstm_on_rd
      && isatty (fileno (stream)) && __flslbbuf ())
    return __set_errno (EINVAL);

  return getc_unlocked (stream);
}
strong_alias (fgetc, fgetc_unlocked)
strong_alias (fgetc, getc)

