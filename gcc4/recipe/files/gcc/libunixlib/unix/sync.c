/* Synchronise unwritten data in buffers to disk.
   Copyright (c) 2004-2010 UnixLib Developers.  */

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <internal/os.h>
#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/fd.h>
#include <pthread.h>

/* This function is always successful.  */
void
sync (void)
{
  /* Ensure data has been written to all files on temporary filing
     system.  */
  (void) __os_args (255, 0, 0, NULL);
}

int
fsync (int fd)
{
  _kernel_oserror *err;
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE_CANCELLATION

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);

  /* Must be only for write */
  if (!(file_desc->fflag & (O_WRONLY | O_RDWR)))
    return __set_errno (EBADF);

  /* Only meaningful for those backed by a read RISC OS file handle */
  if (file_desc->devicehandle->type != DEV_RISCOS)
    return __set_errno (EINVAL);

  /* Ensure data has been written to the file.  */
  err = __os_args (255, (int) file_desc->devicehandle->handle, 0, NULL);
  if (err)
    return __ul_seterr (err, EOPSYS);

  return 0;
}
