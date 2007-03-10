/* Read clock information.
   Copyright (c) 2005 UnixLib Developers.  */

#include <errno.h>
#include <time.h>
#include <unixlib/os.h>

int clock_gettime (clockid_t clk_id, struct timespec *tp)
{
  switch (clk_id)
    {
    case CLOCK_REALTIME:
      {
	unsigned int buf[2], tc;
	int high, low;

	_kernel_oserror *err;
	buf[0] = 3;
	err = __os_word (14, buf);
	if (err)
	  {
	    __ul_seterr (err, 1);
	    return -1;
	  }

	/* This is a derivation of __cvt_riscos_time but without the
	   truncation of centiseconds.  */
	high = buf[1] & 0xff;
	low = buf[0];

	/* Firstly, subtract 0x336e996a00 centiseconds from the
	   RISC OS time.  */
	tc = 0x6e996a00U;
	if (low < tc) /* check for a carry.  */
	  high--;
	low -= tc;

	/* Subtract 0x33 from the fifth byte. We assume that high > 0x33.  */
	high -= 0x33;

	/* Remove the centiseconds from the time.  */

	/* 0x1000000000 / 100 = 42949672.96 */
	tp->tv_nsec = (low / 100) + (high * 42949673U);
	tp->tv_sec = low - (high / 25); /* compensate for 0.04 error.  */
      }
      break;
    case CLOCK_MONOTONIC:
      if (tp != NULL)
	{
	  /* UnixLib's clock function reads RISC OS's Monotonic Timer
	     and has centisecond resolution.  */
	  clock_t c = clock ();
	  tp->tv_sec = c / 100;
	  tp->tv_nsec = (c - (tp->tv_sec * 100)) * 1000000000;
	}
      break;
    default:
      return __set_errno (EINVAL);
    }
  return 0;
}
