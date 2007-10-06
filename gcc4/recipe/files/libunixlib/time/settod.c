/* settimeofday ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <errno.h>
#include <sys/time.h>

/* Set the current time of day and timezone information.  */
int
settimeofday (const struct timeval *tv, const struct timezone *tz)
{
  return __set_errno (EPERM);
}
