/* readv () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/socket.h>

ssize_t
readv (int fd, const struct iovec *vector, int count)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return _sreadv (fd - __FD_SOCKET_OFFSET, vector, count);

  if (count < 0)
    return __set_errno (EINVAL);

  /* Read each buffer, recording how many bytes were read.  */
  size_t bytes_read = 0;
  for (int i = 0; i != count; ++i)
    if (vector[i].iov_len != 0)
      {
	size_t bytes = fread (vector[i].iov_base, 1, vector[i].iov_len, &__iob[fd]);
	/* If we failed on the first read, then return -1, otherwise return
	   the number of bytes we have read.  */
	if (bytes == -1)
	  return bytes_read ? bytes_read : -1;
	bytes_read += bytes;

	/* ssize_t overflow ? */
	if (bytes_read > SSIZE_MAX)
	  return __set_errno (EINVAL);

	/* If we did not read enough bytes, then give up.  */
	if (bytes != vector[i].iov_len)
	  return bytes_read;
      }

  return bytes_read;
}
