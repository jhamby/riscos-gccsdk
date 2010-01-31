/* setgid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Set the real and effective group ID of the process to gid.  */

int
setgid (__gid_t gid)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;

  PTHREAD_UNSAFE

  if (gid == sulproc->gid)
    return 0;
  if (gid == sulproc->egid)
    {
      sulproc->gid = gid;
      return 0;
    }

  return __set_errno (EINVAL);
}
