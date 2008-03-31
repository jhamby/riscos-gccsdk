/* connect ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
connect (int s, const struct sockaddr *name, socklen_t namelen)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _connect ((int)(getfd (s)->devicehandle->handle), name, namelen);
}
