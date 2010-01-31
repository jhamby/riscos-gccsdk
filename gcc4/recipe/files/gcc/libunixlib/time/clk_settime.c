/* clock_settime ()
 * Written by Nick Burrett
 * Copyright (c) 2005-2007 UnixLib Developers
 */

#include <errno.h>
#include <time.h>

int
clock_settime (clockid_t clk_id, const struct timespec *tp)
{
  switch (clk_id)
    {
    case CLOCK_REALTIME:
    case CLOCK_MONOTONIC:
      return __set_errno (EPERM);
    }

  return __set_errno (EINVAL);
}
