/* isgraph ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(isgraph) (int c)
{
  return isgraph (c);
}

int
isgraph_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ~(___ctype_white | ___ctype_ctrl));
}
