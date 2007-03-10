/* chown ()
 * Copyright (c) 2002-2006 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define IGNORE(x) x = x

int
chown (const char *file, uid_t owner, gid_t group)
{
  IGNORE (file);
  IGNORE (owner);
  IGNORE (group);
  return __set_errno (ENOSYS);
}
