/* getuid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <unistd.h>

#include <internal/unix.h>

__uid_t
getuid (void)
{
  return __ul_global.sulproc->uid;
}
