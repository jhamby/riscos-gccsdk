/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/fchdir.c,v $
 * $Date: 2003/06/01 20:47:07 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fchdir.c,v 1.1 2003/06/01 20:47:07 alex Exp $";
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
  file_desc = getfd (fd);

  if (file_desc->devicehandle->type != DEV_RISCOS || (file_desc->dflag & FILE_ISDIR) == 0)
    return __set_errno (ENOTDIR);

  name = ((DIR *)(file_desc->devicehandle->handle))->dd_name_can;

  return chdir (name);
}
