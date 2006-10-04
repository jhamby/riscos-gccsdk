/* seteuid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
seteuid (__uid_t uid)
{
  PTHREAD_UNSAFE

  if (uid == __proc->euid)
    return 0;
  if (uid == __proc->uid)
    {
      __proc->euid = uid;
      return 0;
    }

  return -1;
}
