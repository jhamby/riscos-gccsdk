/* lseek (), lseek64 ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/fd.h>
#include <pthread.h>

off_t
lseek (int fd, off_t offset, int whence)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);

  /* The validity of whence is check by the device specific operation.  */

  return dev_funcall (file_desc->devicehandle->type, lseek,
		    (file_desc, offset, whence));
}

off_t
lseek64 (int fd, off_t offset, int whence)
{
  return lseek(fd, offset, whence);
}
