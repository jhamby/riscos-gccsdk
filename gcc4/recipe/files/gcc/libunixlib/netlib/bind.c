/* bind ()
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
bind (int s, const struct sockaddr *name, socklen_t namelen)
{
#ifdef __TARGET_SCL__
  return _bind (s, name, namelen);
#else
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _bind ((int)getfd (s)->devicehandle->handle, name, namelen);
#endif
}
