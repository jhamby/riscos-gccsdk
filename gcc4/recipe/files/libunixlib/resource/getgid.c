/* getgid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__gid_t
getgid (void)
{
  return __proc->gid;
}
