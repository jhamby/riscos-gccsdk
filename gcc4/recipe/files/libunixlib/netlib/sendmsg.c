/* sendmsg ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 1995-2008 UnixLib Developers
 */

#include <sys/socket.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
sendmsg (int s, const struct msghdr *msg, int flags)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _sendmsg ((int)(getfd (s)->devicehandle->handle), msg, flags);
}
