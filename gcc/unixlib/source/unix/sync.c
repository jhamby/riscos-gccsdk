/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/sync,v $
 * $Date: 1997/10/19 21:50:58 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sync,v 1.8 1997/10/19 21:50:58 unixlib Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <sys/os.h>
#include <sys/dev.h>
#include <sys/unix.h>

#include <unixlib/fd.h>

int
sync (void)
{
  _kernel_oserror *err;

  /* Ensure data has been written to all files on temporary filing system.  */
  err = os_args (255, 0, 0, NULL);
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

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];
  /* Ensure data has been written to the file.  */
  err = os_args (255, (int) file_desc->handle, 0, NULL);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
