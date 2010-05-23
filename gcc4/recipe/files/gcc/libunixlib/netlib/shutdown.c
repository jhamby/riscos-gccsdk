/* shutdown ()
 * Copyright (c) 1997 Nicholas Clark
 * Copyright (c) 1997-2010 UnixLib Developers
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
shutdown (int s, int how)
{
#ifdef __TARGET_SCL__
  return _shutdown (s, how);
#else
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _shutdown ((int)getfd (s)->devicehandle->handle, how);
#endif
}
