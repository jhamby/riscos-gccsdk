/* UnixLib open() implementation.
   Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005 UnixLib Developers.  */

#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

/* #define DEBUG */

char *
ttyname (int fd)
{
  static char ttyname[] = "/dev/tty";

  PTHREAD_UNSAFE

  if (BADF (fd))
    {
      (void) __set_errno (EBADF);
      return NULL;
    }

  if (getfd (fd)->devicehandle->type == DEV_TTY)
    return ttyname;

  return NULL;
}

/* This function will open a file on the specified file descriptor
   with given RISC OS file handle. Caller assures fd and fh are valid.  */
int
__open_fh (int fd, int fh, int oflag, int mode)
{
  struct __unixlib_fd *file_desc;
  char *rofs;

  PTHREAD_UNSAFE

  file_desc = getfd (fd);
  file_desc->fflag = oflag;
  file_desc->dflag = FILE_HANDLE_FROM_OS; /* Never close this handle. */

  file_desc->devicehandle = __proc->sul_malloc (__proc->pid, __proc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return -1;

  rofs = __fd_to_name (fh, NULL, 0);
  if (rofs == NULL)
    {
      __proc->sul_free (__proc->pid, file_desc->devicehandle);
      file_desc->devicehandle = NULL;
      return -1;
    }
  /* Perform a special check for devices.  */
  file_desc->devicehandle->type = __getdevtype (rofs, __RISCOSIFY_NO_PROCESS);
  file_desc->devicehandle->refcount = 1;
  free (rofs);

  file_desc->devicehandle->handle = (void *) fh;

  return fd;
}

/* This function will open a file on the specified file descriptor
   with given filename. Caller assures fd is valid.  */
int
__open_fn (int fd, const char *file, int oflag, int mode)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  file_desc = getfd (fd);
  file_desc->fflag = oflag;
  file_desc->dflag = 0;

  file_desc->devicehandle = __proc->sul_malloc (__proc->pid, __proc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return -1;

  /* Perform a special check for devices.  */
  file_desc->devicehandle->type = __getdevtype (file, __get_riscosify_control());
  file_desc->devicehandle->refcount = 1;

  /* Perform the device specific open operation.  */
  file_desc->devicehandle->handle = dev_funcall (file_desc->devicehandle->type,
				 open, (file_desc, file, mode));
  if (file_desc->devicehandle->handle == (void *) -1)
    {
      __proc->sul_free (__proc->pid, file_desc->devicehandle);
      file_desc->devicehandle = NULL;
      return -1;
    }

  return fd;
}

int
open (const char *file, int oflag, ...)
{
  va_list ap;
  int mode, fd;

  PTHREAD_UNSAFE_CANCELLATION

  if (file == NULL)
    return __set_errno (EINVAL);

/* __riscosify() is called by __fsopen() */

#ifdef DEBUG
  __os_print ("open(): file = ");
  __os_print (file);
  __os_nl ();
#endif

  if (oflag & O_CREAT)
    {
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  /* Allocate a new file descriptor. This function automatically
     sets errno if there are none available.  */
  fd = __alloc_file_descriptor (0);
  if (fd == -1)
    return -1;

  return __open_fn (fd, file, oflag, mode);
}
