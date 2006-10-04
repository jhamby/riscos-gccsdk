/* umask ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unixlib/types.h>
#include <sys/stat.h>
#include <unixlib/unix.h>
#include <pthread.h>

__mode_t
umask (__mode_t cmode)
{
  __mode_t old_umask;

  PTHREAD_UNSAFE

  old_umask = __proc->umask;

  __proc->umask = cmode & 0777;
  return old_umask;
}
