/* sendto ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 2000-2011 UnixLib Developers
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

ssize_t
sendto (int s, const void *msg, size_t len, int flags,
	const struct sockaddr *to, socklen_t tolen)
{
#ifdef __TARGET_SCL__
  if ((unsigned)s < __FD_SOCKET_OFFSET)
    return __set_errno (EBADF);
  return _sendto (s - __FD_SOCKET_OFFSET, msg, len, flags, to, tolen);
#else
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _sendto ((int)getfd (s)->devicehandle->handle, msg, len, flags, to, tolen);
#endif
}
