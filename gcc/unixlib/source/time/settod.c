/* settimeofday ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <sys/time.h>

/* Set the current time of day and timezone information.  */
int
settimeofday (const struct timeval *tv, const struct timezone *tz)
{
  tv = tv;
  tz = tz;
  (void) __set_errno (EPERM);
  return -1;
}
