/* readlink ()
 * Copyright (c) 2002-2013 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

ssize_t
readlink (const char *path, char *buf, size_t bufsiz)
{
  path = path;
  buf = buf;
  bufsiz = bufsiz;

  return __set_errno (ENOSYS);
}
