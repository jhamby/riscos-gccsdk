/****************************************************************************
 *
 * $Source: $
 * $Date: $
 * $Revision: $
 * $State: $
 * $Author: $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: $";
#endif

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
  file_desc = &__u->fd[fd];

  if (file_desc->device != DEV_RISCOS || (file_desc->dflag & FILE_ISDIR) == 0)
    return __set_errno (ENOTDIR);

  name = ((DIR *)(file_desc->handle))->dd_name_can;

  return chdir (name);
}
