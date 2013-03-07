/* UnixLib fopen(), fopen64() implementation.
   Copyright 2001-2013 UnixLib Developers.  */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <internal/unix.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

FILE *
fopen (const char *filename, const char *mode)
{
  PTHREAD_UNSAFE

  /* No check on filename or mode being NULL or filename being an empty string.
     filename checks will be check in open() and mode shouldn't be checked.  */

#ifdef DEBUG
  debug_printf ("-- fopen(fname=%s, mode=%s): ", filename, mode);
#endif

  const __io_mode m = __getmode (mode);
  if (! m.__bits.__read && ! m.__bits.__write)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Create new file stream.  */
  FILE *stream = __newstream ();
  if (stream == NULL)
    return NULL;

  stream->__mode = m;

  __mode_t file_mode;
  if (m.__bits.__read && m.__bits.__write)
    file_mode = O_RDWR;
  else
    file_mode = m.__bits.__read ? O_RDONLY : O_WRONLY;

  if (m.__bits.__append)
    file_mode |= O_APPEND;
  if (m.__bits.__truncate)
    file_mode |= O_TRUNC;

  int fd;
  if (m.__bits.__create)
    fd = open (filename, file_mode | O_CREAT,
	       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  else
    fd = open (filename, file_mode);

#ifdef DEBUG
  debug_printf ("fd=%d\n", fd);
#endif

  /* When fd is not valid, we can just quit and leave the stream
     invalidated.  This stream will be picked up and reused the next time
     __newstream is called.  */
  if (fd < 0)
    return NULL;

  return __stream_init (fd, stream);
}
#if __UNIXLIB_LFS64_SUPPORT
#  error "64-bit LFS support missing."
#else
/* FIXME: indicate 4 GByte-1 limit.  */
strong_alias (fopen, fopen64)
#endif

