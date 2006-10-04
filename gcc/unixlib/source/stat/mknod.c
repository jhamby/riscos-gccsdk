/* mknod ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <sys/stat.h>

int
mknod (const char *path, mode_t mode, dev_t dev)
{
  path = path;
  mode = mode;
  dev = dev;

  return __set_errno (ENOSYS);
}
