/* gmtime ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2006 UnixLib Developers
 */

#include <time.h>
#include <kernel.h>
#include <swis.h>

struct tm *
gmtime (const time_t *tp)
{
  struct tm *t = __calendar_convert (Territory_ConvertTimeToUTCOrdinals, tp,
				     NULL);

   /* GMT conversion rather than localtime - whether local time is on daylight
      saving is irrelevalant */
   t->tm_gmtoff = 0;
   t->tm_isdst = 0;
   t->tm_zone = "GMT";
   return t;
}
