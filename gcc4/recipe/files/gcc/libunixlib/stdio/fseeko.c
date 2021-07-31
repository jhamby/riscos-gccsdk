/* UnixLib fseeko(), fseeko64() implementation.
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
fseeko (FILE *stream, __off_t offset, int w)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream))
    return __set_errno (EINVAL);

  __off_t new_offset;
  int new_w;
  if (w == SEEK_CUR)
    {
      if (fgetpos (stream, &new_offset) == -1)
	return -1;
      new_offset += offset;
      new_w = SEEK_SET;
    }
  else if (w == SEEK_SET || w == SEEK_END)
    {
      new_offset = offset;
      new_w = w;
    }
  else
    return __set_errno (EINVAL);

  /* Write out any pending data.  */
  if (stream->__mode.__bits.__write && __flsbuf (EOF, stream) == EOF)
    return -1;

  __off_t cur_offset = lseek (fileno (stream), new_offset, new_w);
  if (cur_offset == (__off_t)-1)
    {
      stream->__error = 1;
      return -1;
    }

  stream->__offset = cur_offset;

#ifdef DEBUG
  debug_printf ("-- fseeko(fd=%d): newpos=%u\n", stream->fd, stream->__offset);
#endif

  /* Set the input counter to zero so we will read in
     fresh data upon the next read.  */
  stream->i_cnt = 0;
  /* Discard the pushed back character.  */
  stream->__pushedback = 0;
  /* Since we are moving the file position, we are no longer at EOF.  */
  stream->__eof = 0;

  return 0;
}
#if __UNIXLIB_LFS64_SUPPORT
#  error "64-bit LFS support missing."
#else
int
fseeko64 (FILE *stream, __off64_t offset, int w)
{
  if (offset >= -1U && (__off_t)(offset >> 32) != -1U)
    return __set_errno (EOVERFLOW);
  return fseeko (stream, (__off_t)offset, w);
}
#endif

