/* localtime_r ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2006 UnixLib Developers
 */

#include <time.h>
#include <kernel.h>
#include <swis.h>

struct tm *
localtime_r (const time_t *tp, struct tm *t)
{
  t = __calendar_convert (Territory_ConvertTimeToOrdinals, tp, t);
  tzset(); /* To initialise timezone & daylight.  */
  t->tm_gmtoff = 3600 * daylight - timezone;
  t->tm_isdst = daylight;
  t->tm_zone = (daylight == 0) ? tzname[0] : tzname[1];
  return t;
}
