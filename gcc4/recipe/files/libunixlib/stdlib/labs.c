/* labs ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <stdlib.h>

long
labs (long x)
{
  return (x < 0) ? -x : x;
}
