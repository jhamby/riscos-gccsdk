/* tolower ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>

int
(tolower) (int c)
{
  return tolower (c);
}

int
tolower_l (int c, locale_t locobj)
{
  return (&locobj->ctype_lower[1])[c];
}
