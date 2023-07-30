/* islower ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(islower) (int c)
{
  return islower (c);
}

int
islower_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_lower);
}
