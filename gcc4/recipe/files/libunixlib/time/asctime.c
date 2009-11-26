/* asctime ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2008 UnixLib Developers
 */

#include <time.h>

/* Defined by POSIX as not threadsafe */
char *
asctime (const struct tm *brokentime)
{
  char riscos_time[5];

  __cvt_broken_time (brokentime, riscos_time);
  return __standard_time (riscos_time, NULL);
}

char *
asctime_r (const struct tm *brokentime, char *buffer)
{
  char riscos_time[5];

  __cvt_broken_time (brokentime, riscos_time);
  return __standard_time (riscos_time, buffer);
}
