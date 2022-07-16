/* isprint ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isprint) (int c)
{
  return isprint (c);
}

int
isprint_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] &
          (___ctype_upper | ___ctype_lower | ___ctype_digit | ___ctype_punc |
           ___ctype_white)) && !((&locobj->ctype[1])[c] & ___ctype_ctrl);
}
