/* Truncate a file to a specified length.
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>

#include <internal/dev.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <unixlib/local.h>
#include <internal/fd.h>
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

  err = __os_args (3, (int) file_desc->devicehandle->handle,
		   (int) length, NULL);
  if (err)
    return __ul_seterr (err, EOPSYS);

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
    return __ul_seterr (err, EOPSYS);

  /* Truncate and close it.  */
  err = __os_args (3, fd, (int) length, NULL);
  __os_fclose (fd);

  if (err)
    return __ul_seterr (err, EOPSYS);

  return 0;
}
