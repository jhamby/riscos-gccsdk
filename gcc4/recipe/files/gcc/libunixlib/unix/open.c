/* UnixLib ttyname() and open() implementation.
   Copyright (c) 2000-2010 UnixLib Developers.  */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>

#include <unixlib/local.h>
#include <internal/unix.h>
#include <internal/dev.h>
#include <internal/fd.h>
#include <internal/local.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

/* This function will open a file on the specified file descriptor
   with given RISC OS file handle. Caller assures fd and fh are valid.  */
int
__open_fh (int fd, int fh, int oflag, int mode)
{
  PTHREAD_UNSAFE

  struct __unixlib_fd *file_desc = getfd (fd);
  file_desc->fflag = oflag;
  file_desc->dflag = FILE_HANDLE_FROM_OS; /* Never close this handle. */

  struct __sul_process *sulproc = __ul_global.sulproc;
  file_desc->devicehandle = sulproc->sul_malloc (sulproc->pid,
						 sulproc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return __set_errno (ENOMEM);

  char *rofs = __fd_to_name (fh, NULL, 0);
  if (rofs == NULL)
    {
      sulproc->sul_free (sulproc->pid, file_desc->devicehandle);
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
  PTHREAD_UNSAFE

  struct __unixlib_fd *file_desc = getfd (fd);
  file_desc->fflag = oflag;
  file_desc->dflag = 0;

  struct __sul_process *sulproc = __ul_global.sulproc;
  file_desc->devicehandle = sulproc->sul_malloc (sulproc->pid,
						 sulproc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return __set_errno (ENOMEM);

  /* Perform a special check for devices.  */
  file_desc->devicehandle->type = __getdevtype (file,
						__get_riscosify_control());
  file_desc->devicehandle->refcount = 1;

  /* Perform the device specific open operation.  */
  file_desc->devicehandle->handle = dev_funcall (file_desc->devicehandle->type,
				 open, (file_desc, file, mode));
  if (file_desc->devicehandle->handle == (void *) -1)
    {
      sulproc->sul_free (sulproc->pid, file_desc->devicehandle);
      file_desc->devicehandle = NULL;
      return -1;
    }

  return fd;
}

int
open (const char *file, int oflag, ...)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (file == NULL)
    return __set_errno (EINVAL);

#ifdef DEBUG
  debug_printf ("open(file=%s)\n", file);
#endif

  int mode;
  if (oflag & O_CREAT)
    {
      va_list ap;
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  /* Allocate a new file descriptor. This function automatically
     sets errno if there are none available.  */
  int fd = __alloc_file_descriptor (0);
  if (fd == -1)
    return -1;

  return __open_fn (fd, file, oflag, mode);
}
