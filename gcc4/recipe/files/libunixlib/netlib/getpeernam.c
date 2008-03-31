/* getpeername ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
getpeername (int s, __SOCKADDR_ARG name, socklen_t *__restrict namelen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getpeername ((int)(getfd (s)->devicehandle->handle), name, namelen);
}
