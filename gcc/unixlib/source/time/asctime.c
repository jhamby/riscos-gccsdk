/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/asctime.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: asctime.c,v 1.2 2001/01/29 15:10:22 admin Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>

/* Defined by POSIX as not threadsafe */
char *
asctime (const struct tm *brokentime)
{
  char riscos_time[6];

  __cvt_broken_time (brokentime, riscos_time);
  return __standard_time (riscos_time);
}
