/* Modify file attributes of an open file.
   Copyright (c) 2005, 2008 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <internal/unix.h>
#include <internal/os.h>
#include <internal/dev.h>
#include <internal/swiparams.h>
#include <internal/local.h>
#include <pthread.h>

int fchmod (int fd, mode_t mode)
{
  struct __unixlib_fd *file_desc;
  _kernel_oserror *err;
  int regs[10];
  char name[_POSIX_NAME_MAX];

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  /* Get the file name associated with the file descriptor.  */
  file_desc = getfd (fd);

  if (file_desc->devicehandle->type != DEV_RISCOS)
    return __set_errno (EINVAL);

  if (__fd_to_name ((int) file_desc->devicehandle->handle,
		    name, sizeof (name)) == NULL)
    return __set_errno (EBADF);

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = __os_file (OSFILE_WRITECATINFO_ATTR, name, regs);
  if (err)
    {
      __ul_seterr (err, 0);
      return __set_errno (EPERM);
    }

  return 0;
}
