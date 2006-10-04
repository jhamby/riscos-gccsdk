/* setegid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
setegid (gid_t gid)
{
  PTHREAD_UNSAFE

  if (gid == __proc->egid)
    return 0;
  if (gid == __proc->gid)
    {
      __proc->egid = gid;
      return 0;
    }

  return __set_errno (EPERM);
}
