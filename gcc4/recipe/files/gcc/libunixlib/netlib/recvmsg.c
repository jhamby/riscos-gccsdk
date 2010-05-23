/* recvmsg ()
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
recvmsg (int s, struct msghdr *msg, int flags)
{
#ifdef __TARGET_SCL__
  return _recvmsg (s, msg, flags);
#else
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _recvmsg ((int)(getfd (s)->devicehandle->handle), msg, flags);
#endif
}
