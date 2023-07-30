/* toascii ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>

int
(toascii) (int c)
{
  return toascii (c);
}

int
toascii_l (int c, locale_t locobj)
{
  (void) locobj;
  return toascii (c);
}
