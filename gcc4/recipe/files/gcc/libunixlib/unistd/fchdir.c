/* fchdir ()
 * Written by Alex Waugh
 * Copyright (c) 2003-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <unixlib/local.h>

int fchdir (int fd)
{
  struct __unixlib_fd *file_desc;
  char *name;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  /* Get the dir name associated with the file descriptor.  */
  file_desc = getfd (fd);

  if (file_desc->devicehandle->type != DEV_RISCOS || (file_desc->dflag & FILE_ISDIR) == 0)
    return __set_errno (ENOTDIR);

  name = ((DIR *)(file_desc->devicehandle->handle))->dd_name_can;

  return chdir (name);
}
