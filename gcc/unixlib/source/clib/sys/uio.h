/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/uio.h,v $
 * $Date: 2004/04/17 10:51:15 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __SYS_UIO_H
#define __SYS_UIO_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#include <sys/types.h>

__BEGIN_DECLS

/* Structure describing a section of memory.  */

struct iovec
{
  /* Starting address.  */
  void *iov_base;
  /* Length in bytes.  */
  size_t iov_len;
};

/* Read data from file descriptor FD, and put the result in the
   buffers described by VECTOR, which is a vector of COUNT `struct iovec's.
   The buffers are filled in the order specified.  */
extern ssize_t readv (int __fd, const struct iovec * __vector,
		      int __count) __THROW;

/* Write data pointed by the buffers described by VECTOR, which
   is a vector of COUNT `struct iovec's, to file descriptor FD.
   The data is written in the order specified. */
extern ssize_t writev (int __fd, const struct iovec * __vector,
		       int __count) __THROW;

__END_DECLS

#endif
