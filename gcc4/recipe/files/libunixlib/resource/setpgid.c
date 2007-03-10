/* setpgid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>

int
setpgid (__pid_t pid, __pid_t pgid)
{
  return setpgrp (pid, pgid);
}
