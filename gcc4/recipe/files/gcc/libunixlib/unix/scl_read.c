/* read () for SCL
 * Copyright (c) 2010 UnixLib Developers
 */

#include <sys/socket.h>
#include <unistd.h>

/* FIXME: only supports socket fd ! */

ssize_t
read (int fd, void *buf, size_t nbytes)
{
  return _sread (fd, buf, nbytes);
}

