/* getgid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <unistd.h>

#include <internal/unix.h>

__gid_t
getgid (void)
{
  struct __sul_process *sulproc = __ul_global.sulproc;

  return sulproc->gid;
}
