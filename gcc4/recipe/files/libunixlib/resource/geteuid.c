/* geteuid ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

__uid_t
geteuid (void)
{
  return __ul_global.sulproc->euid;
}
