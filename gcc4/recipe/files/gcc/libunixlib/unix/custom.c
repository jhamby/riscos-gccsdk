/*
 * Implement a custom UnixLib device.
 *
 * This is used where the device specific structures need to set on a per
 * file descriptor basis, such as faking a socket.
 *
 * Copyright (c) 2005-2009 UnixLib Developers
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


/* Return the UnixLib device from a file descriptor.  This allows the
   overriding to occur.  */
struct dev *
__unixlib_getdev(int fd)
{
  struct __unixlib_fd *file_desc;

  file_desc = getfd(fd);
  if (!file_desc || !file_desc->devicehandle) return NULL;

  return file_desc->devicehandle->handle;
}


void *
__customopen (struct __unixlib_fd *fd, const char *file, int mode)
{
  struct dev *cdev = malloc(sizeof(struct dev));

  if (!cdev) return NULL;

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
__customclose (struct __unixlib_fd *fd)
{
  int ret = ((struct dev *)fd->devicehandle->handle)->close(fd);

  if (ret) return ret;

  free(fd->devicehandle->handle);
  fd->devicehandle->handle = NULL;

  return 0;
}


int
__customread (struct __unixlib_fd *fd, void *data, int nbyte)
{
  return ((struct dev *)fd->devicehandle->handle)->read(fd, data, nbyte);
}


int
__customwrite (struct __unixlib_fd *fd, const void *data, int nbyte)
{
  return ((struct dev *)fd->devicehandle->handle)->write(fd, data, nbyte);
}


__off_t
__customlseek (struct __unixlib_fd *fd, __off_t lpos, int whence)
{
  return ((struct dev *)fd->devicehandle->handle)->lseek(fd, lpos, whence);
}


int
 __customioctl (struct __unixlib_fd *fd, unsigned long request, void *arg)
{
  return ((struct dev *)fd->devicehandle->handle)->ioctl(fd, request, arg);
}


int
__customselect (struct __unixlib_fd *file_desc, int fd,
                __fd_set *cread, __fd_set *cwrite, __fd_set *except)
{
  return ((struct dev *)file_desc->devicehandle->handle)->select(file_desc, fd, cread, cwrite, except);
}


int
__customstat (const char *filename, struct stat *buf)
{
  return __nullstat(filename, buf);
}


int
__customfstat (int fd, struct stat *buf)
{
  return __nullfstat(fd, buf);
}


int
__customlstat (const char *filename, struct stat *buf)
{
  return __nullstat(filename, buf);
}

void
__set_customselect(int fd, int (*cselect) (void *__fd, int __fd1, __fd_set *__read,
		       __fd_set *__write, __fd_set *__except))
{
  __unixlib_getdev(fd)->select = cselect;
}
