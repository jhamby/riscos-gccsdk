/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/fd.h,v $
 * $Date: 2002/09/24 21:02:37 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
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

/* Maximum number of file descriptors that a process can have. */
#define MAXFD 64

/* Magic number for file descriptors.  */
#define _FDMAGIC 0xfeabaeca

/* File descriptor structure definition. */
struct __unixlib_fd
{
  unsigned int __magic; /* magic number for descriptor validity */
  unsigned int fs : 8; /* Filing system that file resides on */
  unsigned int device : 8; /* Device type (tty, socket etc. ) */
  unsigned int dflag : 16; /* File descriptor flags.  */
  void *handle; /* device specific field (i.e. socket, file handle) */
  int fflag; /* File status flags (attribs for opening a file) */
  __pid_t pid; /* process ID of the owner */
};

/* Macro for checking file descriptor validity.  */
#define BADF(fdes) (((unsigned int)(fdes) < MAXFD) ? (__u->fd[(fdes)].__magic != _FDMAGIC):1)

#ifdef __UNIXLIB_INTERNALS
/* File descriptor flags.  */

/* Set if file is a directory.  */
#define FILE_ISDIR 0x0001


/* Allocate and initialise a new file descriptor.  */
extern int __alloc_file_descriptor (void);
#endif

__END_DECLS

#endif
