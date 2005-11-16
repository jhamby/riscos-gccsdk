/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/calendconv.c,v $
 * $Date: 2002/12/13 11:07:59 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
__calendar_convert (int swinum, const time_t *tp, struct tm *tz)
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
  tz->tm_sec = ordinals_buffer[1];
  tz->tm_min = ordinals_buffer[2];
  tz->tm_hour = ordinals_buffer[3];
  tz->tm_mday = ordinals_buffer[4];
  tz->tm_mon = ordinals_buffer[5] - 1;
  tz->tm_year = ordinals_buffer[6] - 1900;
  tz->tm_wday = ordinals_buffer[7] - 1;
  tz->tm_yday = ordinals_buffer[8] - 1;

#ifdef DEBUG
  printf ("    sec = %d, min = %d, hour = %d\n", tz->tm_sec,
	  tz->tm_min, tz->tm_hour);
  printf ("    mday = %d, mon = %d, year = %d, wday = %d, yday = %d\n", tz->tm_mday,
	  tz->tm_mon, tz->tm_year, tz->tm_wday, tz->tm_yday);
#endif

  return tz;
}
