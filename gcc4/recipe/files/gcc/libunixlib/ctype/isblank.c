/* isblank ()
 * Copyright (c) 2020 UnixLib Developers
 */

#include <ctype.h>

int
(isblank) (int c)
{
  return isblank (c);
}

int
isblank_l (int c, locale_t locobj)
{
  (void) locobj;
  return isblank (c);
}
