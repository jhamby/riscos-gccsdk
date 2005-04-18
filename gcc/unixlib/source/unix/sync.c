/* Synchronise unwritten data in buffers to disk.
   Copyright (c) 2004, 2005 UnixLib Developers.  */

#include <errno.h>
#include <unistd.h>
#include <unixlib/os.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>
#include <pthread.h>

/* This function is always successful.  */
void sync (void)
{
  /* Ensure data has been written to all files on temporary filing system.  */
  __os_args (255, 0, 0, NULL);
}

int fsync (int fd)
{
  _kernel_oserror *err;
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE_CANCELLATION

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);
  /* Ensure data has been written to the file.  */
  err = __os_args (255, (int) file_desc->devicehandle->handle, 0, NULL);
  if (err)
    {
      __ul_seterr (err, 1);
      return -1;
    }

  return 0;
}
