/* setegid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

int
setegid (gid_t gid)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;

  PTHREAD_UNSAFE

  if (gid == sulproc->egid)
    return 0;
  if (gid == sulproc->gid)
    {
      sulproc->egid = gid;
      return 0;
    }

  return __set_errno (EPERM);
}
