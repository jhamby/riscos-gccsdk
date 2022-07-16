/* isupper ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isupper) (int c)
{
  return isupper (c);
}

int
isupper_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_upper);
}
