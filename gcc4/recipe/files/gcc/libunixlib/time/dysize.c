/* dysize ()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <time.h>

int
dysize (int year)
{
  return __isleap (year) ? 366 : 365;
}
