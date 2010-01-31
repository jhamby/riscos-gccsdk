/* getegid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <unistd.h>

#include <internal/unix.h>

__gid_t
getegid (void)
{
  return __ul_global.sulproc->egid;
}
