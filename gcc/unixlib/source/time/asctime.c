/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/asctime,v $
 * $Date: 1997/10/09 20:00:45 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: asctime,v 1.2 1997/10/09 20:00:45 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>

char *
asctime (const struct tm *brokentime)
{
  char riscos_time[6];

  __cvt_broken_time (brokentime, riscos_time);
  return __standard_time (riscos_time);
}
