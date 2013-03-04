/* fstat (), fstat64 ()
 * Copyright (c) 2000-2013 UnixLib Developers
 */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include <internal/dev.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <unixlib/local.h>
#include <internal/swiparams.h>
#include <pthread.h>

int
fstat (int fd, struct stat *buf)
{
  PTHREAD_UNSAFE

  if (buf == NULL)
    return __set_errno (EINVAL);

  if (BADF (fd))
    return __set_errno (EBADF);

  const struct __unixlib_fd *file_desc = getfd (fd);

  buf->st_dev = file_desc->devicehandle->type;

  /* Perform the device specific open operation.  */
  return dev_funcall (file_desc->devicehandle->type, fstat, (fd, buf));
}

int
fstat64 (int fd, struct stat64 *buf)
{
  /* struct stat64 is currently the same as struct stat.  */
  return fstat (fd, (struct stat *)buf);
}
