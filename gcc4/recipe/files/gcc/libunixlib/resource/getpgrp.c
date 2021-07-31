/* getpgrp ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <unistd.h>

#include <internal/unix.h>

__pid_t
getpgrp (void)
{
  return __ul_global.sulproc->pgrp;
}
