/* setsid ()
 * Copyright (c) 2003-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>
#include <errno.h>

/* Creates a new session if the calling process is not a process group leader */

__pid_t
setsid (void)
{
  return __set_errno (ENOSYS);
}
