/* fchdir ()
 * Written by Alex Waugh
 * Copyright (c) 2003-2010 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <unixlib/local.h>

int
fchdir (int fd)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  /* Get the dir name associated with the file descriptor.  */
  struct __unixlib_fd *file_desc = getfd (fd);

  if (file_desc->devicehandle->type != DEV_RISCOS
      || (file_desc->dflag & FILE_ISDIR) == 0)
    return __set_errno (ENOTDIR);

  const char *name = ((const DIR *)(file_desc->devicehandle->handle))->dd_name_can;

  return chdir (name);
}
