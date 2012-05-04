/* UnixLib internal file descriptor interface definition.
   Copyright (c) 2002-2012 UnixLib Developers.  */

#ifdef __TARGET_SCL__
#  error "SCL has no fd support"
#endif

#ifndef __INTERNAL_FD_H
#define __INTERNAL_FD_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

#define getfd(fdes) ((struct __unixlib_fd *)(void *)(((char *)(__ul_global.sulproc->file_descriptors)) + ((fdes) * __ul_global.sulproc->fdsize)))

/* Beware of backwards compatibility when altering these structures.
   They are referenced by SUL and other processes */

/* Multiple __unixlib_fd structures may point to one of these */
struct __unixlib_fd_handle
{
  unsigned int refcount;
  unsigned int type; /* Device type (tty, socket etc. ), see DEV_* */
  void *handle; /* device specific field (i.e. socket, file handle) */
};

/* File descriptor structure definition. */
struct __unixlib_fd
{
  struct __unixlib_fd_handle *devicehandle;
  unsigned int dflag; /* File descriptor flags.  */
  int fflag; /* File status flags (attribs for opening a file) */
};

/* Macro for checking file descriptor validity.  */
#define BADF(fdes) ((unsigned int)(fdes) >= __ul_global.sulproc->maxfd || getfd (fdes)->devicehandle == NULL)

/* File descriptor flags.  */

/* Set if file is a directory.  */
#define FILE_ISDIR		0x0001
/* Set if handle in devicehandle is from OS (OS_ChangeRedirection).  */
#define FILE_HANDLE_FROM_OS	0x0002

/* Allocate and initialise a new file descriptor, searching from fd start
   onwards.  */
extern int __alloc_file_descriptor (int start);

/* Open a file descriptor with given filename.  Returns the file descriptor
   on success, -1 on failure.  */
extern int __open_fn (int __fd, const char *__file, int __oflag, int __mode);

/* Open a file descriptor with given RISC OS file handle.  Returns the file
   descriptor on success, -1 on failure. UnixLib will never close this
   handle, only read/write from it.  */
extern int __open_fh (int __fd, int __fh, int __oflag, int __mode);

/* Close a file descriptor.  Returns zero on success, -1 on failure.  */
extern int __close (struct __unixlib_fd *file_desc);

/* Re-open a file descriptor.  Returns the file descriptor on success,
   -1 on failure.  */
extern int __reopen (int __fd, const char *__file, int __oflag, ...);

__END_DECLS

#endif
