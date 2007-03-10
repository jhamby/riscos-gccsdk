/* send ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2006 UnixLib Developers
 */

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
send (int s, const void *msg, size_t len, int flags)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _send ((int)(getfd (s)->devicehandle->handle), msg, len, flags);
}
