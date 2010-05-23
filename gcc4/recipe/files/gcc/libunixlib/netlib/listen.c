/* listen ()
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
listen (int s, int backlog)
{
#ifdef __TARGET_SCL__
  return _listen (s, backlog);
#else
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _listen ((int)getfd (s)->devicehandle->handle, backlog);
#endif
}
