/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/fd.h,v $
 * $Date: 2004/10/17 16:24:44 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

/* File descriptor interface definition.  */

#ifndef __UNIXLIB_FD_H
#define __UNIXLIB_FD_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

#ifdef __UNIXLIB_INTERNALS

#define getfd(fdes) ((struct __unixlib_fd *)(void *)(((char *)(__proc->file_descriptors)) + ((fdes) * __proc->fdsize)))

/* Beware of backwards compatibility when altering these structures.
   They are referenced by SUL and other processes */

/* Multiple __unixlib_fd structures may point to one of these */
struct __unixlib_fd_handle {
  unsigned int refcount;
  unsigned int type; /* Device type (tty, socket etc. ) */
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
#define BADF(fdes) (((unsigned int)(fdes) < __proc->maxfd) ? (getfd (fdes)->devicehandle == NULL):1)

/* File descriptor flags.  */

/* Set if file is a directory.  */
#define FILE_ISDIR 0x0001


/* Allocate and initialise a new file descriptor.  */
extern int __alloc_file_descriptor (void);
#endif

__END_DECLS

#endif
