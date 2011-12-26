/* UnixLib fpurge() implementation.
   Copyright (c) 2008-2011 UnixLib Developers.  */

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

/* fpurge discards buffered output.  */
int
fpurge (FILE *stream)
{
  PTHREAD_UNSAFE

  if (ferror (stream))
    return EOF;

#ifdef DEBUG
  debug_printf ("-- fpurge(%d): ", stream->fd);
#endif

  stream->i_cnt = 0;
  stream->__pushedi_cnt = 0;
  stream->__pushedback = 0;

  if (stream->o_base != NULL)
    {
      stream->__offset = 0;
      stream->o_cnt = stream->__bufsize;
      stream->o_ptr = stream->o_base;
    }

  return 0;
}
strong_alias (fpurge, __fpurge)

