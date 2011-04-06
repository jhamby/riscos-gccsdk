/* Synchronise unwritten data in buffers to disk.
   Copyright (c) 2004-2011 UnixLib Developers.  */

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
  (void) SWI_OS_Args_Flush (0);
}

int
fsync (int fd)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (BADF (fd))
    return __set_errno (EBADF);

  struct __unixlib_fd *file_desc = getfd (fd);

  /* Must be only for write.  */
  if (!(file_desc->fflag & (O_WRONLY | O_RDWR)))
    return __set_errno (EBADF);

  /* Only meaningful for those backed by a read RISC OS file handle.  */
  if (file_desc->devicehandle->type != DEV_RISCOS)
    return __set_errno (EINVAL);

  /* Ensure data has been written to the file.  */
  const _kernel_oserror *err;
  if ((err = SWI_OS_Args_Flush ((int) file_desc->devicehandle->handle)) != NULL)
    return __ul_seterr (err, EOPSYS);

  return 0;
}
