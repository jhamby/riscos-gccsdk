/* read () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

ssize_t
read (int fd, void *buf, size_t nbytes)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return _sread (fd - __FD_SOCKET_OFFSET, buf, nbytes);
  return fread (buf, 1, nbytes, &__iob[fd]);
}

