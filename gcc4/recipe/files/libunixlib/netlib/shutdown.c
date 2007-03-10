/* shutdown ()
 * Copyright (c) 1997 Nicholas Clark
 * Copyright (c) 1997-2006 UnixLib Developers
 */

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
shutdown (int ssocket, int how)
{
  PTHREAD_UNSAFE

  if (__socket_valid (ssocket) == -1)
    return -1;

  return _shutdown ((int)(getfd (ssocket)->devicehandle->handle), how);
}
