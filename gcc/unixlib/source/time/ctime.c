/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/ctime.c,v $
 * $Date: 2001/07/23 15:37:49 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctime.c,v 1.3 2001/07/23 15:37:49 admin Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <time.h>
#include <unixlib/local.h>

/* Defined by POSIX as not threadsafe */
char *
ctime (const time_t *timer)
{
  unsigned int riscos_time[2];

  /* Set tzname, timezone and daylight.  */
  tzset ();

  /* Convert calendar time to 5 byte RISC OS time.  */
  __cvt_unix_time (*timer, &riscos_time[1], &riscos_time[0]);
  return __standard_time ((const char *)riscos_time);
}
