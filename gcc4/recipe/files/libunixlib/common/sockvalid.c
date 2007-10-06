/* __socket_valid ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <errno.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* Return 0 if 'socket' is both a valid file descriptor  and refers to a
   socket device.  Sets errno and return -1 if not.  */
int __socket_valid (int socket)
{
  PTHREAD_UNSAFE

  if (BADF (socket))
    return __set_errno (EBADF);

  if (getfd (socket)->devicehandle->type != DEV_SOCKET)
    return __set_errno (ENOTSOCK);

  return 0;
}
