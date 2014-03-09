/* mktime ()
 * Written by Nick Burrett on 13 July 1997.
 * Copyright (c) 1997-2014 UnixLib Developers
 */

#include <errno.h>
#include <locale.h>
#include <kernel.h>
#include <swis.h>
#include <time.h>

#include <internal/os.h>
#include <internal/local.h>

time_t
mktime (struct tm *brokentime)
{
  const _kernel_oserror *err;
  unsigned int riscos_time[2]; /* UTC */
  unsigned int ordinals[9];
  if ((err = __cvt_broken_time (brokentime, (char *) riscos_time)) != NULL
      || (err = SWI_Territory_ConvertTimeToOrdinals (
#ifdef __TARGET_SCL__
						     -1,
#else
						     __locale_territory[LC_TIME],
#endif
						     riscos_time,
						     ordinals)) != NULL)
    return __ul_seterr (err, ENOSYS);

  brokentime->tm_sec = ordinals[1];
  brokentime->tm_min = ordinals[2];
  brokentime->tm_hour = ordinals[3];
  brokentime->tm_mday = ordinals[4];
  brokentime->tm_mon = ordinals[5] - 1;
  brokentime->tm_year = ordinals[6] - 1900;
  brokentime->tm_wday = ordinals[7] - 1;
  brokentime->tm_yday = ordinals[8] - 1;

  tzset (); /* Initialise daylight, timezone and tzname.  */
  brokentime->tm_isdst = daylight;
#ifndef __TARGET_SCL__
  brokentime->tm_gmtoff = daylight * 3600 - timezone;
  brokentime->tm_zone = (daylight == 0) ? tzname[0] : tzname[1];
#endif

  return __cvt_riscos_time (riscos_time[1], riscos_time[0]);
}
strong_alias (mktime, timelocal)
