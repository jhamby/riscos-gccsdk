/* getsockopt ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2011 UnixLib Developers
 */

#ifndef __TARGET_SCL__
#  include <pthread.h>
#else
#  include <errno.h>
#  include <stdio.h>
#endif
#include <sys/socket.h>

#ifndef __TARGET_SCL__
#  include <internal/unix.h>
#  include <internal/local.h>
#endif

int
getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
#ifdef __TARGET_SCL__
  if ((unsigned)s < __FD_SOCKET_OFFSET)
    return __set_errno (EBADF);
  return _getsockopt (s - __FD_SOCKET_OFFSET, level, optname, optval, optlen);
#else
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getsockopt ((int)getfd (s)->devicehandle->handle, level, optname, optval, optlen);
#endif
}
