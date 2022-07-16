/* isspace ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isspace) (int c)
{
  return isspace (c);
}

int
isspace_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_white);
}
