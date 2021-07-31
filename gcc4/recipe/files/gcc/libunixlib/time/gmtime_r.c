/* gmtime_r ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2011 UnixLib Developers
 */

#include <errno.h>
#include <locale.h>
#include <kernel.h>
#include <swis.h>
#include <time.h>

#include <internal/os.h>
#include <internal/local.h>

/* Note time/gmtime_r.c and time/localtime_r.c are very similar concerning
   implementation.  */

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_Territory_ConvertTimeToUTCOrdinals (int __territory,
					const unsigned int __ro_time[],
					unsigned int __ordinals[])
{
  register int territory __asm ("r0") = __territory;
  register const unsigned int *ro_time __asm ("r1") = __ro_time;
  register unsigned int *ordinals __asm ("r2") = __ordinals;
  register _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_Territory_ConvertTimeToUTCOrdinals]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (territory), "r" (ro_time), "r" (ordinals),
		      [SWI_Territory_ConvertTimeToUTCOrdinals] "i" (Territory_ConvertTimeToUTCOrdinals | (1<<17))
		    : "r14", "cc", "memory");
  return err;
}

struct tm *
gmtime_r (const time_t *tp, struct tm *tz)
{
  /* Convert to 5-byte time.  */
  unsigned int riscos_time[2];
  __cvt_unix_time (*tp, &riscos_time[1], &riscos_time[0]);

  const _kernel_oserror *err;
  unsigned int ordinals[15];
  if ((err = SWI_Territory_ConvertTimeToUTCOrdinals (
#ifdef __TARGET_SCL__
						     -1,
#else
						     __locale_territory[LC_TIME],
#endif
						     riscos_time, ordinals)) != NULL)
    {
      __ul_seterr (err, ENOSYS);
      return NULL;
    }

  static struct tm tz_fixed;
  if (tz == NULL)
    tz = &tz_fixed;
  tz->tm_sec = ordinals[1];
  tz->tm_min = ordinals[2];
  tz->tm_hour = ordinals[3];
  tz->tm_mday = ordinals[4];
  tz->tm_mon = ordinals[5] - 1;
  tz->tm_year = ordinals[6] - 1900;
  tz->tm_wday = ordinals[7] - 1;
  tz->tm_yday = ordinals[8] - 1;

   /* GMT conversion rather than localtime - whether local time is on daylight
      saving is irrelevalant */
   tz->tm_isdst = 0;
#ifndef __TARGET_SCL__
   tz->tm_gmtoff = 0;
   tz->tm_zone = "GMT";
#endif
   return tz;
}
