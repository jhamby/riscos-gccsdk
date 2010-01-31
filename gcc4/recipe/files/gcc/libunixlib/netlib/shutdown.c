/* shutdown ()
 * Copyright (c) 1997 Nicholas Clark
 * Copyright (c) 1997-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
shutdown (int ssocket, int how)
{
  PTHREAD_UNSAFE

  if (__socket_valid (ssocket) == -1)
    return -1;

  return _shutdown ((int)(getfd (ssocket)->devicehandle->handle), how);
}
