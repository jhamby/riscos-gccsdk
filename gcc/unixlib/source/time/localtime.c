/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/localtime.c,v $
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
localtime (const time_t *tp)
{
  struct tm *t = __calendar_convert (Territory_ConvertTimeToOrdinals, tp,
				     &__tz[0]);
  t->tm_gmtoff = 3600 * daylight - timezone;
  t->tm_isdst = daylight;
  t->tm_zone = (daylight == 0) ? tzname[0] : tzname[1];
  return t;
}
