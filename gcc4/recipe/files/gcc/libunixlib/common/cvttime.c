/* __cvt_riscos_time (), __cvt_unix_time ()
 * Copyright (c) 2000-2014 UnixLib Developers
 */

#include <internal/local.h>
#include <unixlib/types.h>

/* Convert RISC OS format 5 byte time into Unix format time.
   Only the lower byte of high will be used.  */
__time_t
__cvt_riscos_time (unsigned int high, unsigned int low)
{
  return __cvt_riscos_time_csec ((__int64_t)low + ((__int64_t)(high & 0xFF) << 32)) / 100;
}

/* Convert Unix time into RISC OS five byte time.  */
void
__cvt_unix_time (__time_t unix_time, unsigned int *high, unsigned int *low)
{
  __int64_t ro_time = __cvt_unix_time_csec ((__int64_t)unix_time * 100);
  *low = (unsigned)ro_time;
  *high = (unsigned)(ro_time >> 32);
}
