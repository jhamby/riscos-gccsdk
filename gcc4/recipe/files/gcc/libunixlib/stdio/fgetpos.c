/* UnixLib fgetpos(), fgetpos64() implementation.
   Copyright 2001-2011 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

int
fgetpos (FILE *stream, __off_t *pos)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || pos == NULL)
    return __set_errno (EINVAL);

  /* It can be difficult to determine the correct file position
     if the file is opened for read/write because we are using
     a separate buffer for read and write operations.  We could
     add an extra field to the FILE structure that would be
     set if the last operation was a write, then we could check
     the appropriate fields.  Instead we will take the easy option
     and flush the write buffers.  */
  if (stream->__mode.__bits.__read
      && stream->__mode.__bits.__write
      && __flsbuf (EOF, stream) == -1)
    return -1;

#ifdef DEBUG
  debug_printf ("-- fgetpos(fd=%d): ", stream->fd);
#endif

  if (stream->i_base)
    {
#ifdef DEBUG
      debug_printf ("offset=%u, i_cnt=%d, pushed_back=%d\n",
		    stream->__offset, stream->i_cnt, stream->__pushedback);
#endif
      *pos = stream->__offset
	       - (stream->__pushedback ? stream->__pushedi_cnt + 1
				       : stream->i_cnt);
    }
  else if (stream->o_base)
    {
#ifdef DEBUG
      debug_printf ("offset=%u, o_ptr - o_base=%d\n",
		    stream->__offset, stream->o_ptr - stream->o_base);
#endif
      *pos = stream->__offset + (stream->o_ptr - stream->o_base);
    }
  else
    {
#ifdef DEBUG
      debug_printf ("offset=%u, pushed_back=%d\n",
		    stream->__offset, stream->__pushedback);
#endif
      *pos = stream->__offset - stream->__pushedback;
    }

  return 0;
}
#if __UNIXLIB_LFS64_SUPPORT
#  error "64-bit LFS support missing."
#else
int
fgetpos64 (FILE *stream, __off64_t *pos)
{
  __off_t pos32;
  if (fgetpos (stream, &pos32) == -1)
    return -1;
  *pos = pos32;
  return 0;
}
#endif

