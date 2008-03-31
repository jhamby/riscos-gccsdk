/* wait ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <sys/wait.h>
#include <sys/types.h>

#include <pthread.h>
#include <internal/unix.h>

/* Wait for a child to die. When one does, put its status in *status
   and return its process ID. For errors, return -1.  */

pid_t
wait (int *status)
{
  PTHREAD_SAFE_CANCELLATION

  return wait4 (WAIT_ANY, status, 0, 0);
}
