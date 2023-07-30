/* isascii ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>

int
(isascii) (int c)
{
  return isascii (c);
}

int
isascii_l (int c, locale_t locobj)
{
  (void) locobj;
  return isascii (c);
}
