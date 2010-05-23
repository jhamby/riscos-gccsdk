/* readv () for SCL
 * Copyright (c) 2010 UnixLib Developers
 */

#include <sys/uio.h>
#include <sys/socket.h>

/* FIXME: only supports socket fd ! */

ssize_t
readv (int fd, const struct iovec *vector, int count)
{
  return _sreadv (fd, vector, count);
}
