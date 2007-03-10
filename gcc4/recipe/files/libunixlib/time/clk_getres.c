/* clock_getres()
 * Copyright (c) 2005-2006 UnixLib Developers
 */

#include <errno.h>
#include <time.h>

int clock_getres (clockid_t clk_id, struct timespec *res)
{
  switch (clk_id)
    {
    case CLOCK_MONOTONIC:
    case CLOCK_REALTIME:
      /* Both the monotonic and realtime clocks have centisecond
	 resolution.  */
      if (res != NULL)
	{
	  res->tv_sec = 0;
	  res->tv_nsec = 1 * 1000000000;
	}
      break;
    default:
      return __set_errno (EINVAL);
    }
  return 0;
}
