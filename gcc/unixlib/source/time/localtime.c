/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/localtime.c,v $
 * $Date: 2001/08/08 08:45:06 $
 * $Revision: 1.3.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: localtime.c,v 1.3.2.1 2001/08/08 08:45:06 admin Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <kernel.h>
#include <swis.h>

struct tm *
localtime (const time_t *tp)
{
  struct tm *t = __calendar_convert (Territory_ConvertTimeToOrdinals, tp);
  t->tm_gmtoff = 3600 * daylight - timezone;
  t->tm_isdst = daylight;
  t->tm_zone = (daylight == 0) ? tzname[0] : tzname[1];
  return t;
}
