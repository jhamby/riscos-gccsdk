/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/uio.h,v $
 * $Date: 2001/09/14 14:01:17 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __SYS_UIO_H
#define __SYS_UIO_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_size_t
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

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
extern __ssize_t readv (int __fd, const struct iovec * __vector, int __count);

/* Write data pointed by the buffers described by VECTOR, which
   is a vector of COUNT `struct iovec's, to file descriptor FD.
   The data is written in the order specified. */
extern __ssize_t writev (int __fd, const struct iovec * __vector, int __count);

#ifdef __cplusplus
}
#endif

#endif
