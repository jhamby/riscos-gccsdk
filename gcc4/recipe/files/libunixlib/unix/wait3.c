/* wait3 ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

/* Wait for a child to die. When one does, put its status in *status
   and return its process ID. For errors, return -1.  */

pid_t
wait3 (int *status, int options, struct rusage *usage)
{
  return wait4 (WAIT_ANY, status, options, usage);
}
