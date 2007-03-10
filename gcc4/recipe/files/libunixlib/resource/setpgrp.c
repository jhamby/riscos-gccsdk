/* setpgrp ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
setpgrp (__pid_t pid, __pid_t pgid)
{
  PTHREAD_UNSAFE
 
  if (__proc->pid == pid)
    __proc->pgrp = pgid;
  return 0;
}
