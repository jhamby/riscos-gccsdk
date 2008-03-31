/* sendto ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
sendto (int s, const void *msg, size_t len, int flags,
	const struct sockaddr *to, socklen_t tolen)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _sendto ((int)(getfd (s)->devicehandle->handle), msg, len, flags, to, tolen);
}
