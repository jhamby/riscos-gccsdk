/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/ioctl.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ioctl.c,v 1.4 2003/04/05 09:33:57 alex Exp $";
#endif

#include <errno.h>

#include <sys/ioctl.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>
#include <pthread.h>

int
ioctl (int fd, unsigned long request, void *arg)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (!arg)
    return __set_errno (EINVAL);

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];

  return __funcall ((*__dev[file_desc->device].ioctl),
		    (file_desc, request, arg));
}
