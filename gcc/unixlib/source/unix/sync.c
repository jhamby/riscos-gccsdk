/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/sync.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sync.c,v 1.4 2003/04/05 09:33:57 alex Exp $";
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

  file_desc = getfd (fd);
  /* Ensure data has been written to the file.  */
  err = __os_args (255, (int) file_desc->devicehandle->handle, 0, NULL);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
