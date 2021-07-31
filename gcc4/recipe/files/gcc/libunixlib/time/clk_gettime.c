/* Read clock information.
   Copyright (c) 2005-2013 UnixLib Developers.  */

#include <errno.h>
#include <kernel.h>
#include <time.h>

#include <internal/os.h>
#include <internal/local.h>

int
clock_gettime (clockid_t clk_id, struct timespec *tp)
{
  if (tp == NULL)
    return __set_errno (EFAULT);

  switch (clk_id)
    {
      case CLOCK_REALTIME:
        {
	  unsigned int buf[2];
	  buf[0] = 3;
	  if (_kernel_osword (14, (int *)buf) < 0)
	    return -1;

	  /* Convert RISC OS time to Unix time (with csec resolution).  */
	  __int64_t csec = __cvt_riscos_time_csec (((__int64_t)(buf[1] & 0xFF) << 32)
						   + buf[0]);
	  tp->tv_sec = csec / 100;
	  tp->tv_nsec = csec % 100;
	  if (tp->tv_nsec < 0)
	    {
	      tp->tv_nsec = 100 + tp->tv_nsec;
	      tp->tv_sec -= 1;
	    }
	  tp->tv_nsec *= 10000000;
          break;
        }

      case CLOCK_MONOTONIC:
	{
	  /* UnixLib's clock function reads RISC OS's Monotonic Timer
	     and has centisecond resolution.  */
	  clock_t c = clock ();
	  tp->tv_sec = c / 100;
	  tp->tv_nsec = (c - tp->tv_sec * 100) * 10000000;
	  break;
	}

      default:
        return __set_errno (EINVAL);
    }

  return 0;
}
