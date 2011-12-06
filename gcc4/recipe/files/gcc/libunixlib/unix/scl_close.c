/* close () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

int
close (int fd)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return _sclose (fd - __FD_SOCKET_OFFSET);
  return fclose (&__iob[fd]);
}
