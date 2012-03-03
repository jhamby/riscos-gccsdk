/* __get_ro_socket ()
 * Copyright 2012 UnixLib Developers.
 */

#include <errno.h>
#ifndef __TARGET_SCL__
#  include <internal/dev.h>
#  include <internal/unix.h>
#endif
#include <internal/local.h>

int
__get_ro_socket (int sockfd)
{
#ifdef __TARGET_SCL__
  if ((unsigned)sockfd < __FD_SOCKET_OFFSET)
    return __set_errno (EBADF);
  return sockfd - __FD_SOCKET_OFFSET;
#else
  return (int)getfd (sockfd)->devicehandle->handle;
#endif
}

