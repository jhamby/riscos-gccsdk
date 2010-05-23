/* ctime ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <time.h>

/* Defined by POSIX as not threadsafe */
char *
ctime (const time_t *timer)
{
  return ctime_r (timer, NULL);
}
