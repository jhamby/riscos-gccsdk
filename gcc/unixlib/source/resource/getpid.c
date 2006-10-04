/* getpid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__pid_t
getpid (void)
{
  return __proc->pid;
}
