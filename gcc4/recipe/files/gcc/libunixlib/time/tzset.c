/* tzset ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>
#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <string.h>
#include <swis.h>
#include <time.h>

#ifndef __TARGET_SCL__
#  include <internal/unix.h>
#endif
#include <internal/os.h>

#ifndef __TARGET_SCL__
/* The default timezones.  */
static char __tzname[2][8] = { "GMT", "BST" };
char *tzname[2] = { __tzname[0], __tzname[1] };
#endif

/* System V compatibility variables.  */
int timezone = 0, daylight = 0;

void
tzset (void)
{
#ifndef __TARGET_SCL__
  PTHREAD_UNSAFE
#endif

  /* Get timezone information for current territory.  */
  _kernel_swi_regs regs;
#ifndef __TARGET_SCL__
  regs.r[0] = __locale_territory[LC_TIME];
#else
  regs.r[0] = -1; /* Current territory.  */
#endif
  int nondstoffset;
  if (_kernel_swi (Territory_ReadTimeZones, &regs, &regs) == NULL)
    {
      /* regs[0] contains a pointer to the name of the standard time zone.
	 regs[1] contains a pointer to the name of the daylight saving time.
	 We copy these strings to be safe if their location should change. */
#ifndef __TARGET_SCL__
      strncpy (__tzname[0], (const char *) regs.r[0], 7);
      __tzname[0][7] = '\0';
      strncpy (__tzname[1], (const char *) regs.r[1], 7);
      __tzname[1][7] = '\0';
#endif
      nondstoffset = regs.r[2];
    }
  else
    {
#ifndef __TARGET_SCL__
      strcpy (__tzname[0], "GMT");
      strcpy (__tzname[1], "BST");
#endif
      nondstoffset = 0;
    }
#ifndef __TARGET_SCL__
  tzname[0] = __tzname[0];
  tzname[1] = __tzname[1];
#endif

  /* To determine if we are in DST, we need to read the offset of the
     current time zone and compare against the non-DST version.  */
  if (_kernel_swi (Territory_ReadCurrentTimeZone, &regs, &regs) == NULL)
    daylight = (nondstoffset == regs.r[1]) ? 0 : 1;
  else
    daylight = 0;

  timezone = -(regs.r[1] / 100);
  /* Add 1 hour if we're in daylight saving time.  */
  if (daylight)
    timezone += 3600 * daylight;
}
