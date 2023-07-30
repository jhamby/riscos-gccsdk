/* isxdigit ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isxdigit) (int c)
{
  return isxdigit (c);
}

int
isxdigit_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_xdigit);
}
