/* abs ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>

int abs (int x)
{
  return (x < 0) ? -x : x;
}
