/* geteuid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__uid_t
geteuid (void)
{
  return __proc->euid;
}
