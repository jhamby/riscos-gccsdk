/* dup ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <fcntl.h>
#include <unistd.h>

int
dup (int fd)
{
  return fcntl (fd, F_DUPFD, 0);
}
