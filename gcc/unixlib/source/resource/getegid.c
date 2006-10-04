/* getegid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__gid_t
getegid (void)
{
  return __proc->egid;
}
