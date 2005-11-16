/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/gmtime.c,v $
 * $Date: 2002/12/13 11:07:59 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <kernel.h>
#include <swis.h>

struct tm *
gmtime (const time_t *tp)
{
  struct tm *t = __calendar_convert (Territory_ConvertTimeToUTCOrdinals, tp,
				     &__tz[0]);

   /* GMT conversion rather than localtime - whether local time is on daylight
      saving is irrelevalant */
   t->tm_gmtoff = 0;
   t->tm_isdst = 0;
   t->tm_zone = "GMT";
   return t;
}
