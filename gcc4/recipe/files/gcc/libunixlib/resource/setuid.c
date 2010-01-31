/* setuid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

int
setuid (__uid_t uid)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;

  PTHREAD_UNSAFE

  if (uid == sulproc->uid)
    return 0;
  if (uid == sulproc->euid)
    {
      sulproc->uid = uid;
      return 0;
    }

  return __set_errno (EINVAL);
}
