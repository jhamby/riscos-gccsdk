/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/calendconv.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.3.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: calendconv.c,v 1.3.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

/* Territory time support, written by Nick Burrett on 13 July 1997.  */

#include <time.h>
#include <locale.h>
#include <string.h>
#include <unixlib/os.h>
#include <swis.h>
#include <unixlib/local.h>

/* #define DEBUG */

#ifdef DEBUG
#include <stdio.h>
#endif

/* Common function for localtime() and gmtime().  */
struct tm *
__calendar_convert (int swinum, const time_t *tp)
{
  unsigned int riscos_time[2];
  unsigned int ordinals_buffer[15];
  int regs[10];

  /* Convert to 5-byte time.  */
  __cvt_unix_time (*tp, &riscos_time[1], &riscos_time[0]);

#ifdef DEBUG
  printf ("__calendar_convert:\n");
  printf ("    *tp = %x, riscos_time[1] = %x, riscos_time[0] = %x\n",
	  *tp, riscos_time[1], riscos_time[0]);
#endif

  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)ordinals_buffer;
  __os_swi (swinum, regs);
  __tz->tm_sec = ordinals_buffer[1];
  __tz->tm_min = ordinals_buffer[2];
  __tz->tm_hour = ordinals_buffer[3];
  __tz->tm_mday = ordinals_buffer[4];
  __tz->tm_mon = ordinals_buffer[5] - 1;
  __tz->tm_year = ordinals_buffer[6] - 1900;
  __tz->tm_wday = ordinals_buffer[7] - 1;
  __tz->tm_yday = ordinals_buffer[8] - 1;

#ifdef DEBUG
  printf ("    sec = %d, min = %d, hour = %d\n", __tz->tm_sec,
	  __tz->tm_min, __tz->tm_hour);
  printf ("    mday = %d, mon = %d, year = %d, wday = %d, yday = %d\n", __tz->tm_mday,
	  __tz->tm_mon, __tz->tm_year, __tz->tm_wday, __tz->tm_yday);
#endif

  return __tz;
}
