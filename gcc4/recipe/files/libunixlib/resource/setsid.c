/* setsid ()
 * Copyright (c) 2003-2008 UnixLib Developers
 */

#include <unistd.h>
#include <errno.h>

#include <internal/unix.h>

/* Creates a new session if the calling process is not a process group leader */

__pid_t
setsid (void)
{
  return __set_errno (ENOSYS);
}
