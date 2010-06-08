/* Modify file attributes of an open file.
   Copyright (c) 2005-2010 UnixLib Developers.  */

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

int
fchmod (int fd, mode_t mode)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  /* Get the file name associated with the file descriptor.  */
  struct __unixlib_fd *file_desc = getfd (fd);

  if (file_desc->devicehandle->type != DEV_RISCOS)
    return __set_errno (EINVAL);

  char name[_POSIX_NAME_MAX];
  if (__fd_to_name ((int) file_desc->devicehandle->handle,
		    name, sizeof (name)) == NULL)
    return __set_errno (EBADF);

  /* Set the file access permission bits.  */
  int regs[10];
  regs[5] = __set_protection (mode);
  _kernel_oserror *err;
  if ((err = __os_file (OSFILE_WRITECATINFO_ATTR, name, regs)) != NULL)
    return __ul_seterr (err, EPERM);

  return 0;
}
