/* __calendar_convert ()
 * Written by Nick Burrett on 13 July 1997.
 * Copyright (c) 1997-2008 UnixLib Developers
 */

#include <time.h>
#include <locale.h>
#include <string.h>
#include <swis.h>

#include <internal/os.h>
#include <internal/local.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

static struct tm __tz[1];

/* Common function for localtime(), localtime_r(), gmtime() and gmtime_r().
   When called with tz, a ptr to a static struct is returned which makes
   this routine non-reentrant.  */
struct tm *
__calendar_convert (int swinum, const time_t *tp, struct tm *tz)
{
  unsigned int riscos_time[2];
  unsigned int ordinals_buffer[15];
  int regs[10];

  /* Convert to 5-byte time.  */
  __cvt_unix_time (*tp, &riscos_time[1], &riscos_time[0]);

#ifdef DEBUG
  debug_printf ("__calendar_convert:\n"
		"    *tp = %x, riscos_time[1] = %x, riscos_time[0] = %x\n",
		*tp, riscos_time[1], riscos_time[0]);
#endif

  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)ordinals_buffer;
  __os_swi (swinum, regs);

  if (tz == NULL)
    tz = __tz;
  tz->tm_sec = ordinals_buffer[1];
  tz->tm_min = ordinals_buffer[2];
  tz->tm_hour = ordinals_buffer[3];
  tz->tm_mday = ordinals_buffer[4];
  tz->tm_mon = ordinals_buffer[5] - 1;
  tz->tm_year = ordinals_buffer[6] - 1900;
  tz->tm_wday = ordinals_buffer[7] - 1;
  tz->tm_yday = ordinals_buffer[8] - 1;

#ifdef DEBUG
  debug_printf ("    sec = %d, min = %d, hour = %d\n", tz->tm_sec,
		tz->tm_min, tz->tm_hour);
  debug_printf ("    mday = %d, mon = %d, year = %d, wday = %d, yday = %d\n",
		tz->tm_mday, tz->tm_mon, tz->tm_year, tz->tm_wday, tz->tm_yday);
#endif

  return tz;
}
