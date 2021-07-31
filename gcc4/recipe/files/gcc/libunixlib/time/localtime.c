/* localtime ()
 * Written by Nick Burrett on 12 July 1997.
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <time.h>

struct tm *
localtime (const time_t *tp)
{
  return localtime_r (tp, NULL);
}
