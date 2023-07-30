/* toupper ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(toupper) (int c)
{
  return toupper (c);
}

int
toupper_l (int c, locale_t locobj)
{
  return (&locobj->ctype_upper[1])[c];
}
