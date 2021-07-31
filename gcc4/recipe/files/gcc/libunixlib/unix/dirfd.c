/* Implements dirfd.
   Copyright (c) 2008 UnixLib Developers.  */

#include <dirent.h>
#include <fcntl.h>

#include <internal/unix.h>
#include <unixlib/local.h>
#include <pthread.h>

int
dirfd (DIR *dirp)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (dirp->fd != -1)
    return dirp->fd;

  return dirp->fd = open (dirp->dd_name_can, O_RDONLY);
}
