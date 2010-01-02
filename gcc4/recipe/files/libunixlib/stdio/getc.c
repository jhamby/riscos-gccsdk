/* UnixLib fgetc(), getc() and getchar() implementation.
   Copyright 2000-2010 UnixLib Developers.  */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

#undef getchar

int
fgetc (FILE * stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || !stream->__mode.__bits.__read)
    return __set_errno (EINVAL);

  /* When we have possibly unflushed data in one of our line buffered streams
     and we're reading from a tty attached stream, flush all those streams.  */
  if (__ul_global.fls_lbstm_on_rd && isatty (fileno (stream)) && __flslbbuf ())
    return __set_errno (EINVAL);

  return getc_unlocked (stream);
}
strong_alias (fgetc, getc)

int
getchar (void)
{
  return fgetc (stdin);
}
