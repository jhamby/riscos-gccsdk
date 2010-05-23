/* write () for SCL
 * Copyright (c) 2010 UnixLib Developers
 */

#include <sys/socket.h>
#include <unistd.h>

/* FIXME: only supports socket fd ! */

ssize_t
write (int fd, const void *buf, size_t nbytes)
{
  return _swrite (fd, buf, nbytes);
}

