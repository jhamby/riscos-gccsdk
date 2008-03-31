/* getsockname ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
getsockname (int s, struct sockaddr *name, socklen_t *namelen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getsockname ((int)(getfd (s)->devicehandle->handle), name, namelen);
}
