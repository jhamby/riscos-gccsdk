/* chown ()
 * Copyright (c) 2002-2007 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int
chown (const char *file, uid_t owner, gid_t group)
{
  return __set_errno (ENOSYS);
}
