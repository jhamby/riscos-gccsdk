/* lseek (), lseek64 ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/fd.h>

__off_t
lseek (int fd, __off_t offset, int whence)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  struct __unixlib_fd *file_desc = getfd (fd);

  /* The validity of whence is check by the device specific operation.  */

  return dev_funcall (file_desc->devicehandle->type, lseek,
		      (file_desc, offset, whence));
}

__off64_t
lseek64 (int fd, __off64_t offset, int whence)
{
#if __UNIXLIB_LFS64_SUPPORT
#  error "64-bit LFS support missing."
#else
  /* Check if the __off64_t offset fits in a __off_t one.  Further
     checking will be done in lseek itself.  */
  if (offset != (__off64_t)(__off_t)offset)
    return __set_errno (EOVERFLOW);
#endif

  return lseek (fd, (__off_t) offset, whence);
}
