/* fchown ()
 * Copyright (c) 2002-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include <internal/unix.h>
#include <internal/dev.h>
#include <internal/fd.h>
#include <pthread.h>

int
fchown (int fd, uid_t owner, gid_t group)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  /* Get the file name associated with the file descriptor.  */
  file_desc = getfd (fd);

  if (file_desc->devicehandle->type != DEV_RISCOS)
    return __set_errno (EINVAL);

  return __set_errno (ENOSYS);
}
