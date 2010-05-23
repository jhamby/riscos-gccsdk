/* gmtime_r ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <kernel.h>
#include <swis.h>
#include <time.h>

struct tm *
gmtime_r (const time_t *tp, struct tm *resultp)
{
  struct tm *t = __calendar_convert (Territory_ConvertTimeToUTCOrdinals, tp,
				     resultp);

   /* GMT conversion rather than localtime - whether local time is on daylight
      saving is irrelevalant */
   t->tm_isdst = 0;
#ifndef __TARGET_SCL__
   t->tm_gmtoff = 0;
   t->tm_zone = "GMT";
#endif
   return t;
}
