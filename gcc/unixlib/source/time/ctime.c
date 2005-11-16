/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/ctime.c,v $
 * $Date: 2003/04/05 09:33:56 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
