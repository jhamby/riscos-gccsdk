/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/tzset,v $
 * $Date: 1997/10/09 20:00:47 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tzset,v 1.4 1997/10/09 20:00:47 unixlib Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/os.h>
#include <sys/swis.h>

/* The default timezones.  */
char *tzname[2] = { (char *)"GMT", (char *)"BST" };
struct tm __tz[1];

/* System V compatibility variables.  */
int timezone, daylight;

void
tzset (void)
{
  int regs[10];

  /* Use current territory.  */
  regs[0] = __locale_territory[LC_TIME];
  /* Get timezone information.  */
  os_swi (Territory_ReadTimeZones, regs);
  /* regs[0] contains a pointer to the name of the standard time zone.
     regs[1] contains a pointer to the name of the daylight saving time.
     We just store a copy of these pointers because their contents are
     unlikely to be overwritten.  */
  tzname[0] = (char *)regs[0];
  tzname[1] = (char *)regs[1];

  /* To determine if we are in DST, we need to read the current time
     zone and compare against the non DST version.  */
  os_swi (Territory_ReadCurrentTimeZone, regs);
  if (strcmp (tzname[0], (const char *)regs[0]) == 0)
    daylight = 0;
  else
    daylight = 1;

  timezone = -(regs[1] / 100);
  /* Add 1 hour if we're in daylight saving time.  */
  if (daylight)
    timezone += 1*60*60;
}
