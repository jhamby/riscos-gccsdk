/* setuid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
setuid (__uid_t uid)
{
  PTHREAD_UNSAFE
  
  if (uid == __proc->uid)
    return 0;
  if (uid == __proc->euid)
    {
      __proc->uid = uid;
      return 0;
    }
  return -1;
}
