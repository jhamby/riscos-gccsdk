/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/uio,v $
 * $Date: 1997/10/15 22:27:44 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_UIO_H
#define __SYS_UIO_H 1

#ifndef __STDDEF_H
#include <stddef.h>
#endif
#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

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
