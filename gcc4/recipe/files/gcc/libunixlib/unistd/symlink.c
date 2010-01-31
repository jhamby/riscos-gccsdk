/* symlink ()
 * Copyright (c) 2002-2006 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

int
symlink (const char *origfile, const char *newfile)
{
  origfile = origfile;
  newfile = newfile;

  return __set_errno (ENOSYS);
}
