/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/sync.c,v $
 * $Date: 2002/02/14 15:56:39 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sync.c,v 1.3 2002/02/14 15:56:39 admin Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <unixlib/os.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>
#include <pthread.h>

int
sync (void)
{
  _kernel_oserror *err;

  /* Ensure data has been written to all files on temporary filing system.  */
  err = __os_args (255, 0, 0, NULL);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}

int
fsync (int fd)
{
  _kernel_oserror *err;
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];
  /* Ensure data has been written to the file.  */
  err = __os_args (255, (int) file_desc->handle, 0, NULL);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
