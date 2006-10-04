/* getppid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__pid_t
getppid (void)
{
  return __proc->ppid;
}
