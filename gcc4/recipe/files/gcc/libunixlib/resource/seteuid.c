/* seteuid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

int
seteuid (__uid_t uid)
{
  struct __sul_process *sulproc = __ul_global.sulproc;

  PTHREAD_UNSAFE

  if (uid == sulproc->euid)
    return 0;
  if (uid == sulproc->uid)
    {
      sulproc->euid = uid;
      return 0;
    }

  return __set_errno (EINVAL);
}
