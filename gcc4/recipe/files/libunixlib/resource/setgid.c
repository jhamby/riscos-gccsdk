/* setgid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* Set the real and effective group ID of the process to gid.  */

int
setgid (__gid_t gid)
{
  PTHREAD_UNSAFE
  
  if (gid == __proc->gid)
    return 0;
  if (gid == __proc->egid)
    {
      __proc->gid = gid;
      return 0;
    }
  return -1;
}
