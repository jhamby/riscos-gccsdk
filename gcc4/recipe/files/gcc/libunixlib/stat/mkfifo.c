/* mkfifo ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <sys/stat.h>

int
mkfifo (const char *path, mode_t mode)
{
  path = path;
  mode = mode;

  return __set_errno (ENOSYS);
}
