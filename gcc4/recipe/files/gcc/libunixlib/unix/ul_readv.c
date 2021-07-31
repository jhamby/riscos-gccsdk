/* readv () for UnixLib
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/fd.h>

/* Read data from file descriptor FD, and put the result in the
   buffers described by VECTOR, which is a vector of COUNT `struct iovec's.
   The buffers are filled in the order specified.  */

ssize_t
readv (int fd, const struct iovec *vector, int count)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (count < 0)
    return __set_errno (EINVAL);
  if (BADF (fd))
    return __set_errno (EBADF);

  struct __unixlib_fd *file_desc = getfd (fd);

  /* Confirm that the file is open for reading.  */
  if ((file_desc->fflag & O_ACCMODE) == O_WRONLY)
    return __set_errno (EBADF);

  unsigned int device = file_desc->devicehandle->type;

  /* When we're reading from a socket, we can take a short-cut.  */
  if (device == DEV_SOCKET)
    {
      __u->usage.ru_oublock++;
      return _sreadv ((int)file_desc->devicehandle->handle, vector, count);
    }

  /* Read each buffer, recording how many bytes were read.  */
  size_t bytes_read = 0;
  for (int i = 0; i != count; ++i)
    if (vector[i].iov_len != 0)
      {
	__u->usage.ru_inblock++;
	size_t bytes = dev_funcall (device, read,
				    (file_desc, vector[i].iov_base, vector[i].iov_len));
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
