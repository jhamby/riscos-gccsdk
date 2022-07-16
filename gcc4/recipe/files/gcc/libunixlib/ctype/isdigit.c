/* isdigit ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isdigit) (int c)
{
  return isdigit (c);
}

int
isdigit_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_digit);
}
