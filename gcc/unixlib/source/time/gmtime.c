/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/gmtime.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: gmtime.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
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
