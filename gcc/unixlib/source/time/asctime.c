/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/asctime.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: asctime.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
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
