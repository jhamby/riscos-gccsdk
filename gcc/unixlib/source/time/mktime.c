/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/mktime.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mktime.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
#endif

/* Territory time support, written by Nick Burrett on 13 July 1997.  */

#include <locale.h>
#include <time.h>
#include <string.h>
#include <sys/os.h>
#include <sys/swis.h>
#include <unixlib/local.h>

time_t
mktime (const struct tm *brokentime)
{
  unsigned int riscos_time[2];
  int regs[10];
  unsigned int ordinals_buffer[15];

  tzset ();
  __cvt_broken_time (brokentime, (char *)riscos_time);

  /* Normalize the brokentime structure.  */
  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)ordinals_buffer;
  os_swi (Territory_ConvertTimeToOrdinals, regs);
  __tz->tm_wday = ordinals_buffer[7];
  __tz->tm_yday = ordinals_buffer[8];

  /* Read the current time zone and compare it with the
     non DST name. If equivalent, then we're not in DST mode.  */
  os_swi (Territory_ReadCurrentTimeZone, regs);
  __tz->tm_gmtoff = regs[1] / 100;
  strcpy (__tz->tm_zone, (const char *)regs[0]);
  if (strcmp (tzname[0], __tz->tm_zone) == 0)
    __tz->tm_isdst = 0;
  else
    __tz->tm_isdst = 1;

  return __cvt_riscos_time (riscos_time[1], riscos_time[0]);
}
