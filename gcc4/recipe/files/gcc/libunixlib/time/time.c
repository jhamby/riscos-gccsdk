/* Get time in seconds.
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <kernel.h>
#include <time.h>

#include <internal/os.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

time_t
time (time_t *timep)
{
  time_t time1;
  unsigned int buf[2];
  buf[0] = 3;
  if (_kernel_osword (14, (int *)buf) < 0)
    time1 = (time_t) -1;
  else
    {
      time1 = __cvt_riscos_time (buf[1] & 0xff, buf[0]);
#ifdef DEBUG
      debug_printf ("time():  t1 = %x, b[1] = %x, b[0] = %x\n",
		    time1, buf[1], buf[0]);
#endif
    }

  if (timep != NULL)
    *timep = time1;

  return time1;
}
