/* Truncate a file to a specified length.
   Copyright (c) 2005-2011 UnixLib Developers.  */

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
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  struct __unixlib_fd *file_desc = getfd (fd);
  if (file_desc->devicehandle->type != DEV_RISCOS)
    return __set_errno (EOPNOTSUPP);

  const _kernel_oserror *err;
  if ((err = SWI_OS_Args_SetExtent ((int) file_desc->devicehandle->handle,
				    (int) length)) != NULL)
    return __ul_seterr (err, EOPSYS);

  return 0;
}

int
truncate (const char *ux_file, off_t length)
{
  if (ux_file == NULL)
    return __set_errno (EINVAL);

  char file[_POSIX_PATH_MAX];
  if (!__riscosify_std (ux_file, 0, file, sizeof (file), NULL))
    return __set_errno (ENAMETOOLONG);

  /* Open the file.  */
  int fd;
  const _kernel_oserror *err;
  err = __os_fopen (0xc0, file, &fd);
  if (err)
    return __ul_seterr (err, EOPSYS);

  /* Truncate and close it.  */
  err = SWI_OS_Args_SetExtent (fd, (int) length);
  __os_fclose (fd);
  if (err)
    return __ul_seterr (err, EOPSYS);

  return 0;
}
