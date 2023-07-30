/* lseek (), lseek64 () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef LSEEK_FUNCTION
#define LSEEK_FUNCTION lseek
#endif

off_t
LSEEK_FUNCTION (int fd, off_t offset, int whence)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return __set_errno (ENOSYS);
  int r;
  if ((r = fseeko (&__iob[fd], offset, whence)) == -1)
    return r;
  return ftello (&__iob[fd]);
}
