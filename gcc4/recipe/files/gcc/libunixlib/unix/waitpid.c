/* waitpid ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stddef.h>

#include <pthread.h>
#include <internal/unix.h>

/* Wait for a child matching pid to die.

   If pid > 0, match any process whose process ID is pid.
   If pid = -1, match any process.
   If pid = 0, match any process with the same process group
   as the current process.
   If pid < -1, match any process whose process group is the
   absolute value of pid.

   If the WNOHANG bit is set in options, and that child
   is not already dead, return (pid_t) 0.

   If successful, return pid and store the dead child's status in stat_loc.

   Return -1 for errors.

   If the WUNTRACED bit is set in OPTIONS, return status for stopped
   children; otherwise don't.  */

pid_t
waitpid (pid_t pid, int *stat_loc, int options)
{
  PTHREAD_SAFE_CANCELLATION

  return wait4 (pid, stat_loc, options, NULL);
}
