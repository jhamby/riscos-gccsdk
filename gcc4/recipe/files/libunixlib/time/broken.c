/* __cvt_broken_time ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2008 UnixLib Developers
 */

#include <time.h>
#include <locale.h>
#include <swis.h>

#include <internal/os.h>
#include <unixlib/local.h>

/* Convert broken local time to 5-byte RISC OS time (UTC).  */
void
__cvt_broken_time (const struct tm *brokentime, char *riscos_time)
{
  unsigned int ordinals_buffer[7];
  int regs[10];

  ordinals_buffer[0] = 0;
  ordinals_buffer[1] = brokentime->tm_sec;
  ordinals_buffer[2] = brokentime->tm_min;
  ordinals_buffer[3] = brokentime->tm_hour;
  ordinals_buffer[4] = brokentime->tm_mday;
  /* There seems to be a problem in the UK module 0.23 that it
     can't handle out-of-bounds values for months in its
     ConvertOrdinalsToTime implementation. */
  ordinals_buffer[5] = (brokentime->tm_mon % 12) + 1;
  ordinals_buffer[6] = brokentime->tm_year
    + (brokentime->tm_mon / 12)
    + 1900;
  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)riscos_time;
  regs[2] = (int)ordinals_buffer;
  __os_swi (Territory_ConvertOrdinalsToTime, regs);
}
