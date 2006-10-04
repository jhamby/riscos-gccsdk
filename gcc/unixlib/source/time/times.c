/* times ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2006 UnixLib Developers
 */

#include <time.h>
#include <sys/times.h>

clock_t
times (struct tms *tmsp)
{
  clock_t utime = clock ();
  if (tmsp)
    {
      tmsp->tms_utime = utime;	/* user time */
      tmsp->tms_stime = 0;	/* system time */
      tmsp->tms_cutime = 0;	/* user time, children */
      tmsp->tms_cstime = 0;	/* system time, children */
    }
  return utime;
}
