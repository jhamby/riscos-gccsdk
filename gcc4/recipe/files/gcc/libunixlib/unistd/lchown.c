/* lchown ()
 * Written by Alex Waugh
 * Copyright (c) 2006-2007 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int
lchown (const char *file, uid_t owner, gid_t group)
{
  return __set_errno (ENOSYS);
}
