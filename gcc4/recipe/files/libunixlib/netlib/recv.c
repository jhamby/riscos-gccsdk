/* recv ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
recv (int s, void *msg, size_t len, int flags)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _recv ((int)(getfd (s)->devicehandle->handle), msg, len, flags);
}
