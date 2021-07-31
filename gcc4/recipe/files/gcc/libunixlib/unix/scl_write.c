/* write () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

ssize_t
write (int fd, const void *buf, size_t nbytes)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return _swrite (fd - __FD_SOCKET_OFFSET, buf, nbytes);
  return fwrite (buf, 1, nbytes, &__iob[fd]);
}

