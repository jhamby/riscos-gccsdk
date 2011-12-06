/* writev () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/socket.h>

ssize_t
writev (int fd, const struct iovec *vector, int count)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return _swritev (fd - __FD_SOCKET_OFFSET, vector, count);

  if (count < 0)
    return __set_errno (EINVAL);

  /* Write each buffer, recording how many bytes were written.  */
  size_t bytes_written = 0;
  for (int i = 0; i != count; ++i)
    if (vector[i].iov_len != 0)
      {
	size_t bytes = fwrite (vector[i].iov_base, 1, vector[i].iov_len, &__iob[fd]);
	/* If we failed on the first write, then return -1, otherwise return
	   the number of bytes we have written.  */
	if (bytes == -1)
	  return bytes_written ? bytes_written : -1;
	bytes_written += bytes;

	/* ssize_t overflow ? */
	if (bytes_written > SSIZE_MAX)
	  return __set_errno (EINVAL);

	/* If we did not write enough bytes, then give up.  */
	if (bytes != vector[i].iov_len)
	  return bytes_written;
      }

  return bytes_written;
}
