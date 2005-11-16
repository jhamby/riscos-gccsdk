/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/fchown.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>

#include <unixlib/dev.h>
#include <sys/types.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>
#include <pthread.h>

#define IGNORE(x) x = x

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

  IGNORE (owner);
  IGNORE (group);
  return __set_errno (ENOSYS);
}
