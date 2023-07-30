/* iscntrl ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(iscntrl) (int c)
{
  return iscntrl (c);
}

int
iscntrl_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_ctrl);
}
