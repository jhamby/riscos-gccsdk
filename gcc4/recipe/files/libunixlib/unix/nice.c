/* nice ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>
#include <sys/resource.h>

int
nice (int increment)
{
  int old = getpriority (PRIO_PROCESS, 0);
  return setpriority (PRIO_PROCESS, 0, old + increment);
}
