/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/localtime,v $
 * $Date: 1997/10/09 20:00:46 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: localtime,v 1.2 1997/10/09 20:00:46 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <sys/swis.h>

struct tm *
localtime (const time_t *tp)
{
  return __calendar_convert (Territory_ConvertTimeToOrdinals, tp);
}
