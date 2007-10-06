/* getgid ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__gid_t
getgid (void)
{
  struct __sul_process *sulproc = __ul_global.sulproc;

  return sulproc->gid;
}
