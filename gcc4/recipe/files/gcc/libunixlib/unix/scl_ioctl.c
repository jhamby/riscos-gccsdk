/* ioctl () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <termios.h>

int
ioctl (int fd, unsigned long request, void *arg)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return _sioctl (fd - __FD_SOCKET_OFFSET, request, arg);

  /* Very limited support for non-socket fd.
     Dummy TIOCGETA/TIOCSETA support for stdin.  */
  if (fd == 0 && (request == TIOCGETA || request == TIOCSETA))
    return 0;

  return __set_errno (EBADF);
}
