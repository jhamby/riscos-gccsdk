/* ispunct ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(ispunct) (int c)
{
  return ispunct (c);
}

int
ispunct_l (int c, locale_t locobj)
{
  return ((&locobj->ctype[1])[c] & ___ctype_punc);
}
