/* read () for UnixLib
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/fd.h>

ssize_t
read (int fd, void *buf, size_t nbytes)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (nbytes == 0)
    return 0;
  if (BADF (fd))
    return __set_errno (EBADF);

  struct __unixlib_fd *file_desc = getfd (fd);

  /* Confirm that the file is open for reading.  */
  if ((file_desc->fflag & O_ACCMODE) == O_WRONLY)
    return __set_errno (EBADF);

  /* Increment the number of times we have had to read from a device.  */
  __u->usage.ru_inblock++;

  /* Perform the device specific read operation.  */
  return dev_funcall (file_desc->devicehandle->type, read,
		    (file_desc, buf, nbytes));
}
