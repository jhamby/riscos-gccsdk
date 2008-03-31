/* Get time in seconds.
   Copyright (c) 2005, 2007, 2008 UnixLib Developers.  */

#include <time.h>

#include <internal/os.h>
#include <internal/local.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

time_t
time (time_t *timep)
{
  unsigned int buf[2];
  _kernel_oserror *err;
  time_t time1;

  buf[0] = 3;
  if ((err = __os_word (14, buf)) != NULL)
    return __ul_seterr (err, 1);

  time1 = __cvt_riscos_time (buf[1] & 0xff, buf[0]);

#ifdef DEBUG
  debug_printf ("time():  t1 = %x, b[1] = %x, b[0] = %x\n",
		time1, buf[1], buf[0]);
#endif

  if (timep != NULL)
    *timep = time1;

  return time1;
}
