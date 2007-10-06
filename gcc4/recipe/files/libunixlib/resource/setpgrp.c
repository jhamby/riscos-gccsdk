/* setpgrp ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <unistd.h>
#include <pthread.h>
#include <unixlib/unix.h>

int
setpgrp (__pid_t pid, __pid_t pgid)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;

  PTHREAD_UNSAFE
 
  if (sulproc->pid == pid)
    sulproc->pgrp = pgid;

  return 0;
}
