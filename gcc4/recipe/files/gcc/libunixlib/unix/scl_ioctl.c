/* ioctl () for SCL
 * Copyright (c) 2010 UnixLib Developers
 */

#include <sys/ioctl.h>
#include <sys/socket.h>

/* FIXME: only supports socket fd ! */

int
ioctl (int fd, unsigned long request, void *arg)
{
  return _sioctl (fd, request, arg);
}
