/* Read clock information.
   Copyright (c) 2005-2008 UnixLib Developers.  */

#include <errno.h>
#include <stdint.h>
#include <time.h>

#include <internal/os.h>

int
clock_gettime (clockid_t clk_id, struct timespec *tp)
{
  switch (clk_id)
    {
      case CLOCK_REALTIME:
        {
	  unsigned int buf[2];
	  uint64_t centisec, sec;
	  _kernel_oserror *err;

	  buf[0] = 3;
	  if ((err = __os_word (14, buf)) != NULL)
	    return __ul_seterr (err, 1);

	  /* The number of centiseconds that have elapsed between the starts
	     of RISC OS and Unix times is 0x336e996a00.  */
	  centisec = (uint64_t)buf[0] + (((uint64_t)(buf[1] & 0xFF))<<32)
		     - 0x336e996a00ULL;

	  tp->tv_sec = (centisec / 10ULL) / 10ULL;
	  tp->tv_nsec = (centisec - sec * 100ULL) * 10000000;
          break;
        }

    case CLOCK_MONOTONIC:
      if (tp != NULL)
	{
	  /* UnixLib's clock function reads RISC OS's Monotonic Timer
	     and has centisecond resolution.  */
	  clock_t c = clock ();
	  tp->tv_sec = c / 100;
	  tp->tv_nsec = (c - (tp->tv_sec * 100)) * 10000000;
	}
      break;

    default:
      return __set_errno (EINVAL);
    }

  return 0;
}
