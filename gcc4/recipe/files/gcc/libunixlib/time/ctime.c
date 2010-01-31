/* ctime ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2008 UnixLib Developers
 */

#include <time.h>

#include <internal/local.h>

/* Defined by POSIX as not threadsafe */
char *
ctime (const time_t *timer)
{
  unsigned int riscos_time[2];

  /* Set tzname, timezone and daylight.  */
  tzset ();

  /* Convert calendar time to 5 byte RISC OS time.  */
  __cvt_unix_time (*timer, &riscos_time[1], &riscos_time[0]);
  return __standard_time ((const char *)riscos_time, NULL);
}

char *
ctime_r (const time_t *timer, char *buffer)
{
  unsigned int riscos_time[2];

  /* Set tzname, timezone and daylight.  */
  tzset ();

  /* Convert calendar time to 5 byte RISC OS time.  */
  __cvt_unix_time (*timer, &riscos_time[1], &riscos_time[0]);
  return __standard_time ((const char *)riscos_time, buffer);
}
