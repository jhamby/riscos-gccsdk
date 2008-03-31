/* getsockopt ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getsockopt ((int)(getfd (s)->devicehandle->handle), level, optname, optval, optlen);
}
