/* isalpha ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isalpha) (int c)
{
  return isalpha (c);
}

int
isalpha_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_alpha);
}
