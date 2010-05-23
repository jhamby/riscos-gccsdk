/* recv ()
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

ssize_t
recv (int s, void *msg, size_t len, int flags)
{
#ifdef __TARGET_SCL__
  return _recv (s, msg, len, flags);
#else
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _recv ((int)(getfd (s)->devicehandle->handle), msg, len, flags);
#endif
}
