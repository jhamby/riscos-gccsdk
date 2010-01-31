/* llabs ()
 * Copyright (c) 2005-2006 UnixLib Developers
 */

#include <stdlib.h>

long long llabs (long long x)
{
  return (x < 0) ? -x : x;
}
