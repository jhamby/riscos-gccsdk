/* __cvt_riscos_time (), __cvt_unix_time ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <unixlib/local.h>
#include <unixlib/types.h>

/* Time conversion functions.  */

/* Unix time (time_t) represents the number of seconds elapsed since
   00:00:00 on January 1, 1970 Coordinated Universal Time.

   RISC OS time is a 5 byte number representing the number of
   centiseconds that have elapsed since 00:00:00 on January 1, 1900 CUT.

   The number of centiseconds that have elapsed between the starts
   of RISC OS and Unix times is 0x336e996a00.  */

/* Convert RISC OS format 5 byte time into Unix format time.  */
__time_t
__cvt_riscos_time (unsigned int high, unsigned int low)
{
  unsigned int tc;

  high = high & 0xff;

  /* Firstly, subtract 0x336e996a00 centiseconds from the RISC OS time.  */
  tc = 0x6e996a00U;
  if (low < tc) /* check for a carry.  */
    high--;
  low -= tc;

  /* Subtract 0x33 from the fifth byte. We assume that high > 0x33.  */
  high -= 0x33;

  /* Remove the centiseconds from the time.  */
  low = (low / 100) + (high * 42949673U); /* 0x1000000000 / 100 = 42949672.96 */
  low -= (high / 25); /* compensate for 0.04 error.  */

  /* Sorted.  */
  return low;
}

/* Convert Unix time into RISC OS five byte time.  */
void
__cvt_unix_time (__time_t unix_time, unsigned int *high, unsigned int *low)
{
  unsigned int ra,rb,rc,rd,re;

  /* Multiply unix_time by 100.  */
  ra = unix_time;
  rb = 100; /* ls 16 bits of rb. */

  rd = ra >> 16; /* ms 16 bits of ra.  */
  ra = (ra << 16) >> 16;
  /* ra &= ~(rd << 16);*/ /* ls 16 bits of ra.  */

  rc = ra * rb; /* low partial product */
  rb = rd * rb; /* middle partial product */

  /* add middle partial product. look out for carry. */
  rd = rb >> 16;
  re = rc;
  rc += rb << 16;
  if (rc < re)
    rd += 1; /* carry occurred.  */

  /* Add the csecs from 1900 to 1970. */
/*  rd += 0x33; */
  /* look out for a carry.  */
  re = rc;
  rc += 0x6e996a00U;
  rd += (rc < re) ? 0x34 : 0x33;
#if 0
  if (rc < re)
    rd += 1; /* carry occurred */
#endif

  *high = rd & 0xff;
  *low = rc;
}
