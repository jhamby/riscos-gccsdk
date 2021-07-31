/* shutdown ()
 * Copyright (c) 1997 Nicholas Clark
 * Copyright (c) 1997-2011 UnixLib Developers
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

int
shutdown (int s, int how)
{
#ifdef __TARGET_SCL__
  if ((unsigned)s < __FD_SOCKET_OFFSET)
    return __set_errno (EBADF);
  return _shutdown (s - __FD_SOCKET_OFFSET, how);
#else
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _shutdown ((int)getfd (s)->devicehandle->handle, how);
#endif
}
