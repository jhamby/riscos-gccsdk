/* lseek (), lseek64 () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

__off_t
lseek (int fd, __off_t offset, int whence)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return __set_errno (ENOSYS);
  int r;
  if ((r = fseeko (&__iob[fd], offset, whence)) == -1)
    return r;
  return ftello (&__iob[fd]);
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

