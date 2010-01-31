
/* UnixLib statfs()/fstatfs() implementation.
   Copyright (c) 2008 UnixLib Developers.  */

#include <errno.h>
#include <sys/statfs.h>

int
statfs (const char *file, struct statfs *buf)
{
  return __set_errno (ENOSYS);
}

int
fstatfs (int fd, struct statfs *buf)
{
  return __set_errno (ENOSYS);
}
