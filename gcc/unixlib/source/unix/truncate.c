/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/truncate.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: truncate.c,v 1.4 2003/04/05 09:33:57 alex Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <unixlib/dev.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <sys/types.h>

#include <unixlib/local.h>
#include <unixlib/fd.h>
#include <pthread.h>

int
ftruncate (int fd, off_t length)
{
  struct __unixlib_fd *file_desc;
  _kernel_oserror *err;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);
  if (file_desc->devicehandle->type != DEV_RISCOS)
    return __set_errno (EOPNOTSUPP);

  err = __os_args (3, (int) file_desc->devicehandle->handle, (int) length, NULL);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}

int
truncate (const char *ux_file, off_t length)
{
  int fd;
  _kernel_oserror *err;
  char file[_POSIX_PATH_MAX];

  if (ux_file == NULL)
    return __set_errno (EINVAL);

  if (!__riscosify_std (ux_file, 0, file, sizeof (file), NULL))
    return __set_errno (ENAMETOOLONG);

  /* Open the file.  */
  err = __os_fopen (0xc0, file, &fd);
  if (err)
    {
      __seterr (err);
      return -1;
    }

  /* Truncate and close it.  */
  err = __os_args (3, fd, (int) length, NULL);
  __os_fclose (fd);

  if (err)
    {
      __seterr (err);
      return -1;
    }

  return 0;
}
