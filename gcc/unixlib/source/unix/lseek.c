/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/lseek.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lseek.c,v 1.4 2003/04/05 09:33:57 alex Exp $";
#endif

#include <errno.h>
#include <unistd.h>

#include <unixlib/dev.h>
#include <sys/types.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>
#include <pthread.h>

off_t
lseek (int fd, off_t offset, int whence)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];

  /* The validity of whence is check by the device specific operation.  */

  return __funcall ((*(__dev[file_desc->device].lseek)),
		    (file_desc, offset, whence));
}

off_t
lseek64 (int fd, off_t offset, int whence)
{
  return lseek(fd, offset, whence);
}