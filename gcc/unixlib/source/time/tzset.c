/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/tzset.c,v $
 * $Date: 2002/02/14 15:56:38 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tzset.c,v 1.4 2002/02/14 15:56:38 admin Exp $";
#endif

/* Territory time support, written by Nick Burrett on 12 July 1997.  */

#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unixlib/os.h>
#include <swis.h>
#include <pthread.h>

/* The default timezones.  */
static char __tzname[2][8] = { "GMT", "BST" };
char *tzname[2] = { __tzname[0], __tzname[1] };
struct tm __tz[1];

/* System V compatibility variables.  */
int timezone = 0, daylight = 0;

void
tzset (void)
{
  int regs[10];
  int nondstoffset;

  PTHREAD_UNSAFE

  /* Use current territory.  */
  regs[0] = __locale_territory[LC_TIME];
  /* Get timezone information.  */
  __os_swi (Territory_ReadTimeZones, regs);
  /* regs[0] contains a pointer to the name of the standard time zone.
     regs[1] contains a pointer to the name of the daylight saving time.
      We copy these strings to be safe if their location should change. */
  strncpy (__tzname[0], (char *) regs[0], 8);
  __tzname[0][7] = '\0';
  tzname[0] = __tzname[0];
  strncpy (__tzname[1], (char *) regs[1], 8);
  __tzname[1][7] = '\0';
  tzname[1] = __tzname[1];
  nondstoffset = regs[2];


  /* To determine if we are in DST, we need to read the offset of the
     current time zone and compare against the non-DST version.  */
  __os_swi (Territory_ReadCurrentTimeZone, regs);
  daylight = (nondstoffset == regs[1]) ? 0 : 1;

  timezone = -(regs[1] / 100);
  /* Add 1 hour if we're in daylight saving time.  */
  if (daylight)
    timezone += 3600 * daylight;
}
