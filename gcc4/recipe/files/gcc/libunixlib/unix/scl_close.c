/* close () for SCL
 * Copyright (c) 2010 UnixLib Developers
 */

#include <unistd.h>
#include <sys/socket.h>

/* FIXME: only supports socket fd ! */

int
close (int fd)
{
  return _sclose (fd);
}
