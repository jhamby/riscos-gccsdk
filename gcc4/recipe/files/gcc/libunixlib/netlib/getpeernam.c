/* getpeername ()
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

int
getpeername (int s, struct sockaddr *name, socklen_t *namelen)
{
#ifdef __TARGET_SCL__
  return _getpeername (s, name, namelen);
#else
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getpeername ((int)getfd (s)->devicehandle->handle, name, namelen);
#endif
}
