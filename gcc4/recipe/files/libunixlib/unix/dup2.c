/* dup2 ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <internal/unix.h>

int
dup2 (int fd1, int fd2)
{
  int save;

  if ((unsigned int) fd2 >= __ul_global.sulproc->maxfd)
    return __set_errno (EINVAL);

  if (fcntl (fd1, F_GETFL) < 0)
    return -1;

  if (fd1 == fd2)
    return fd2;

  /* Close the new file descriptor, if it is open.  */
  save = errno;
  close (fd2);
  (void) __set_errno (save);

  /* Duplicate the old file descriptor.  */
  return fcntl (fd1, F_DUPFD, fd2);
}
