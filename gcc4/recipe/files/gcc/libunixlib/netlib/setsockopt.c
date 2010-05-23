/* setsockopt ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2010 UnixLib Developers
 */

#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <sys/socket.h>

#ifndef __TARGET_SCL__
#  include <internal/unix.h>
#  include <internal/local.h>
#endif

int
setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
#ifdef __TARGET_SCL__
  return _setsockopt (s, level, optname, optval, optlen);
#else
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _setsockopt ((int)getfd (s)->devicehandle->handle, level, optname, optval, optlen);
#endif
}
