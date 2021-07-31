/* gmtime ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <time.h>
#include <swis.h>

struct tm *
gmtime (const time_t *tp)
{
  return gmtime_r (tp, NULL);
}
