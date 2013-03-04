/*
 * Implement a custom UnixLib device.
 *
 * This is used where the device specific structures need to set on a per
 * file descriptor basis, such as faking a socket.
 *
 * Copyright (c) 2005-2013 UnixLib Developers
 */

#include <swis.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <internal/os.h>
#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/swiparams.h>


void *
__customopen (struct __unixlib_fd *file_desc, const char *file, int mode)
{
  struct dev *cdev = malloc (sizeof(struct dev));

  if (!cdev)
    return NULL;

  cdev->open   = __nullopen;
  cdev->close  = __nullclose;
  cdev->read   = __nullread;
  cdev->write  = __nullwrite;
  cdev->lseek  = __nulllseek;
  cdev->ioctl  = __nullioctl;
  cdev->select = __nullselect;
  cdev->stat   = __nullstat;
  cdev->fstat  = __nullfstat;

  return cdev;
}


int
__customclose (struct __unixlib_fd *file_desc)
{
  const struct dev *cdev = (const struct dev *)file_desc->devicehandle->handle;

  int ret = cdev->close (file_desc);
  if (ret)
    return ret;

  free (file_desc->devicehandle->handle);
  file_desc->devicehandle->handle = NULL;

  return 0;
}


int
__customread (struct __unixlib_fd *file_desc, void *data, int nbyte)
{
  const struct dev *cdev = (const struct dev *)file_desc->devicehandle->handle;

  return cdev->read (file_desc, data, nbyte);
}


int
__customwrite (struct __unixlib_fd *file_desc, const void *data, int nbyte)
{
  const struct dev *cdev = (const struct dev *)file_desc->devicehandle->handle;

  return cdev->write (file_desc, data, nbyte);
}


__off_t
__customlseek (struct __unixlib_fd *file_desc, __off_t lpos, int whence)
{
  const struct dev *cdev = (const struct dev *)file_desc->devicehandle->handle;

  return cdev->lseek (file_desc, lpos, whence);
}


int
 __customioctl (struct __unixlib_fd *file_desc, unsigned long request, void *arg)
{
  const struct dev *cdev = (const struct dev *)file_desc->devicehandle->handle;

  return cdev->ioctl (file_desc, request, arg);
}


int
__customselect (struct __unixlib_fd *file_desc, int fd,
                __fd_set *cread, __fd_set *cwrite, __fd_set *except)
{
  const struct dev *cdev = (const struct dev *)file_desc->devicehandle->handle;

  return cdev->select (file_desc, fd, cread, cwrite, except);
}


int
__set_customselect (int fd, int (*cselect) (void *, int, __fd_set *,
		    __fd_set *, __fd_set *))
{
  if (BADF (fd))
    return __set_errno (EBADF);

  struct dev *cdev = (struct dev *)getfd (fd)->devicehandle->handle;
  cdev->select = (int (*)(struct __unixlib_fd *, int, fd_set *, fd_set *,
		   fd_set *)) cselect;
  return 0;
}
