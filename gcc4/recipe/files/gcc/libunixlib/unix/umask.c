/* umask ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <sys/stat.h>

#include <unixlib/types.h>
#include <internal/unix.h>
#include <pthread.h>

__mode_t
umask (__mode_t cmode)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;
  __mode_t old_umask;

  PTHREAD_UNSAFE

  old_umask = sulproc->umask;

  sulproc->umask = cmode & 0777;
  return old_umask;
}
