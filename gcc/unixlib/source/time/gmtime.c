/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/gmtime,v $
 * $Date: 1997/10/09 20:00:46 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: gmtime,v 1.3 1997/10/09 20:00:46 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <sys/swis.h>

struct tm *
gmtime (const time_t *tp)
{
  struct tm *t = __calendar_convert (Territory_ConvertTimeToUTCOrdinals, tp);
   t->tm_gmtoff = 0;
   /* GMT conversion rather than localtime - whether local time is on daylight
    * saving is irrelevalant */
   return t;
}
