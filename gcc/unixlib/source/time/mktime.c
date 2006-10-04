/* mktime ()
 * Written by Nick Burrett on 13 July 1997.
 * Copyright (c) 1997-2006 UnixLib Developers
 */

#include <locale.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <unixlib/os.h>
#include <swis.h>
#include <unixlib/local.h>

time_t
mktime (struct tm *brokentime)
{
  unsigned int riscos_time[2]; /* UTC */
  int regs[10];

  tzset ();
  __cvt_broken_time (brokentime, (char *) riscos_time);

  /* Normalize the brokentime structure.  */
  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int) riscos_time;
  regs[2] = (int) brokentime;
  __os_swi (Territory_ConvertTimeToOrdinals, regs);

  /* Fixup normalised brokentime to lose centiseconds field */
  memmove(brokentime, &brokentime->tm_min, offsetof(struct tm, tm_gmtoff));
  /* And fixup month field */
  brokentime->tm_mon -= 1;
  /* And fixup year field */
  brokentime->tm_year -= 1900;

  /* Set correct timezone information in brokentime structure.  */
  brokentime->tm_gmtoff = daylight * 3600 - timezone;
  brokentime->tm_zone = (daylight == 0) ? tzname[0] : tzname[1];
  brokentime->tm_isdst = daylight;

  return __cvt_riscos_time (riscos_time[1], riscos_time[0]);
}

time_t
timelocal (struct tm *tm)
{
  return mktime (tm);
}
