/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/fchdir.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>

#include <unixlib/dev.h>
#include <unixlib/unix.h>
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
