/* setsockopt ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2006 UnixLib Developers
 */

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _setsockopt ((int)(getfd (s)->devicehandle->handle), level, optname, optval, optlen);
}
