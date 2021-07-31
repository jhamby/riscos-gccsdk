/* localtime_r ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2014 UnixLib Developers
 */

#include <errno.h>
#include <locale.h>
#include <kernel.h>
#include <swis.h>
#include <time.h>

#include <internal/os.h>
#include <internal/local.h>

/* Note time/gmtime_r.c and time/localtime_r.c are very similar concerning
   implementation.  */

struct tm *
localtime_r (const time_t *tp, struct tm *tz)
{
  /* Convert to 5-byte time.  */
  unsigned int riscos_time[2];
  __cvt_unix_time (*tp, &riscos_time[1], &riscos_time[0]);

  const _kernel_oserror *err;
  unsigned int ordinals[9];
  if ((err = SWI_Territory_ConvertTimeToOrdinals (
#ifdef __TARGET_SCL__
						  -1,
#else
						  __locale_territory[LC_TIME],
#endif
						  riscos_time, ordinals)) != NULL)
    {
      __ul_seterr (err, ENOSYS);
      return NULL;
    }

  static struct tm tz_fixed;
  if (tz == NULL)
    tz = &tz_fixed;
  tz->tm_sec = ordinals[1];
  tz->tm_min = ordinals[2];
  tz->tm_hour = ordinals[3];
  tz->tm_mday = ordinals[4];
  tz->tm_mon = ordinals[5] - 1;
  tz->tm_year = ordinals[6] - 1900;
  tz->tm_wday = ordinals[7] - 1;
  tz->tm_yday = ordinals[8] - 1;

  tzset(); /* Initialise daylight, timezone and tzname.  */
  tz->tm_isdst = daylight;
#ifndef __TARGET_SCL__
  tz->tm_gmtoff = 3600 * daylight - timezone;
  tz->tm_zone = (daylight == 0) ? tzname[0] : tzname[1];
#endif
  return tz;
}
