/* fileno ()
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2008-2011 UnixLib developers
 */

#include <stdio.h>
#include <errno.h>

int
fileno (FILE *__stream)
{
  int fd = __stream - &__iob[0];
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return __set_errno (EINVAL);
  return fd;
}

