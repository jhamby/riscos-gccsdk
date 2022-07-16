/* isalnum ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isalnum) (int c)
{
  return isalnum (c);
}

int
isalnum_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & (___ctype_alpha | ___ctype_digit));
}
