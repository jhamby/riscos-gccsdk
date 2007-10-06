/* getppid ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__pid_t
getppid (void)
{
  return __ul_global.sulproc->ppid;
}
