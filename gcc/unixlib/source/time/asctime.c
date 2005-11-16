/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/asctime.c,v $
 * $Date: 2003/04/05 09:33:56 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
