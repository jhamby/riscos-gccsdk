/* __cvt_broken_time ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2011 UnixLib Developers
 */

#include <time.h>
#include <locale.h>
#include <swis.h>

static inline const _kernel_oserror *
SWI_Territory_ConvertOrdinalsToTime (int __territory,
				     char __ro_time[],
				     const unsigned int __ordinals[])
{
  register int territory __asm ("r0") = __territory;
  register char *ro_time __asm ("r1") = __ro_time;
  register const unsigned int ordinals __asm ("r2") = __ordinals;
  register const _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_Territory_ConvertOrdinalsToTime]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (territory), "r" (ro_time), "r" (ordinals),
		      [SWI_Territory_ConvertOrdinalsToTime] "i" (Territory_ConvertOrdinalsToTime | (1<<17))
		    : "r14", "cc", "memory");
}

/* Convert broken local time to 5-byte RISC OS time (UTC).  */
const _kernel_oserror *
__cvt_broken_time (const struct tm *brokentime, char *riscos_time)
{
  const unsigned int ordinals[7] =
    {
      0, /* zero centi secs.  */
      brokentime->tm_sec,
      brokentime->tm_min,
      brokentime->tm_hour,
      brokentime->tm_mday,
      /* There seems to be a problem in the UK module 0.23 that it
	 can't handle out-of-bounds values for months in its
	 ConvertOrdinalsToTime implementation. */
      (brokentime->tm_mon % 12) + 1,
      brokentime->tm_year
	+ (brokentime->tm_mon / 12)
	+ 1900
    };

  return SWI_Territory_ConvertOrdinalsToTime(
#ifdef __TARGET_SCL__
					     -1,
#else
					     __locale_territory[LC_TIME],
#endif
					     riscos_time, ordinals);
}
