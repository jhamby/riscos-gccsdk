/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/mktime,v $
 * $Date: 2000/11/08 10:22:57 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mktime,v 1.4 2000/11/08 10:22:57 admin Exp $";
#endif

/* Territory time support, written by Nick Burrett on 13 July 1997.  */

#include <locale.h>
#include <time.h>
#include <string.h>
#include <sys/os.h>
#include <sys/swis.h>
#include <unixlib/local.h>

time_t
mktime (struct tm *brokentime)
{
  unsigned int riscos_time[2]; /* UTC */
  int regs[10];

  __cvt_broken_time (brokentime, (char *)riscos_time);

  /* Normalize the brokentime structure.  */
  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)brokentime;
  os_swi (Territory_ConvertTimeToOrdinals, regs);

  /* Read the current time zone and compare it with the
     non DST name. If equivalent, then we're not in DST mode.  */
  os_swi (Territory_ReadCurrentTimeZone, regs);
  brokentime->tm_gmtoff = regs[1] / 100;
  strcpy (brokentime->tm_zone, (const char *)regs[0]);
  if (strcmp (tzname[0], brokentime->tm_zone) == 0)
    brokentime->tm_isdst = 0;
  else
    brokentime->tm_isdst = 1;

  return __cvt_riscos_time (riscos_time[1], riscos_time[0]);
}
