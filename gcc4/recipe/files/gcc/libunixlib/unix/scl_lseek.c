/* lseek () for SCL
 * Copyright (c) 2010 UnixLib Developers
 */

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

/* FIXME: fd is a socket handle but lseek is not supported. */

off_t
lseek (int fd, off_t offset, int whence)
{
  return __set_errno (ENOSYS);
}
