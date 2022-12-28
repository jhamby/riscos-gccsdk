/* iswalnum(), iswalpha(), iswcntrl(), iswdigit(), iswgraph(), towlower(),
 * iswprint(), iswpunct(), iswspace(), towupper(), iswxdigit()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <ctype.h>
#include <wctype.h>

int
iswalnum (wint_t wc)
{
  return isalnum (wc);
}

int
iswalpha (wint_t wc)
{
  return isalpha (wc);
}

int
iswcntrl (wint_t wc)
{
  return iscntrl (wc);
}

int
iswdigit (wint_t wc)
{
  return isdigit (wc);
}

int
iswgraph (wint_t wc)
{
  return isgraph (wc);
}

wint_t
towlower (wint_t wc)
{
  return tolower (wc);
}

int
iswprint (wint_t wc)
{
  return isprint (wc);
}

int
iswpunct (wint_t wc)
{
  return ispunct (wc);
}

int
iswspace (wint_t wc)
{
  return isspace (wc);
}

wint_t
towupper (wint_t wc)
{
  return toupper (wc);
}

int
iswxdigit (wint_t wc)
{
  return isxdigit (wc);
}
