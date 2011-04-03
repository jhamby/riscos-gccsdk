/* __standard_time ()
 * Written by Nick Burrett on 13 July 1997.
 * Copyright (c) 1997-2011 UnixLib Developers
 */

#include <errno.h>
#include <kernel.h>
#include <locale.h>
#include <swis.h>
#include <time.h>

#include <internal/os.h>

static __inline__ const _kernel_oserror * __attribute__ ((always_inline))
SWI_Territory_ConvertDateAndTime (int __territory, const char *__ro_time,
				  char *__buf, size_t __size,
				  const char *__str, char **termp)
{
  register int territory __asm ("r0") = __territory;
  register const char *ro_time __asm ("r1") = __ro_time;
  register char *buf __asm ("r2") = __buf;
  register size_t size __asm ("r3") = __size;
  register const char *str __asm ("r4") = __str;
  register const _kernel_oserror *err __asm ("r0");
  register char *term __asm ("r1");
  __asm__ volatile ("SWI\t%[SWI_Territory_ConvertDateAndTime]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err), "=r" (term)
		    : "r" (territory), "r" (ro_time), "r" (size), "r" (str),
		      [SWI_Territory_ConvertDateAndTime] "i" (Territory_ConvertDateAndTime | (1<<17))
		    : "r14", "cc", "memory");
  *termp = term;
  return err;
}

/* Common function for ctime() and asctime().
   Not thread safe, but none of its callers need be either */
char *
__standard_time (const char *riscos_time, char *local_buffer)
{
  static char static_buffer[64];
  char *result = local_buffer ? local_buffer : static_buffer;

  /* Standard time format is: "Tue May 21 13:46:22 1992\n"  */
  char *term;
  const _kernel_oserror *err;
  if ((err = SWI_Territory_ConvertDateAndTime (
#ifdef __TARGET_SCL__
					       -1,
#else
					       __locale_territory[LC_TIME],
#endif
					       riscos_time,
					       result,
					       local_buffer ? 26 : sizeof (static_buffer), /* 26 bytes according to ctime_r, asctime_r man page */
					       "%W3 %M3 %DY %24:%MI:%SE %CE%YR",
					       &term)) != NULL)
    {
      __ul_seterr (err, ENOSYS);
      return NULL;
    }
  *term++ = '\n';
  *term = '\0';

  return result;
}
