/* iswalnum(), iswalpha(), iswcntrl(), iswdigit(), iswgraph(), towlower(),
 * iswprint(), iswpunct(), iswspace(), towupper(), iswxdigit()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>
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

int
iswalnum_l (wint_t wc, locale_t locale)
{
  return isalnum_l (wc, locale);
}

int
iswalpha_l (wint_t wc, locale_t locale)
{
  return isalpha_l (wc, locale);
}

int
iswcntrl_l (wint_t wc, locale_t locale)
{
  return iscntrl_l (wc, locale);
}

int
iswdigit_l (wint_t wc, locale_t locale)
{
  return isdigit_l (wc, locale);
}

int
iswgraph_l (wint_t wc, locale_t locale)
{
  return isgraph_l (wc, locale);
}

int
iswprint_l (wint_t wc, locale_t locale)
{
  return isprint_l (wc, locale);
}

int
iswpunct_l (wint_t wc, locale_t locale)
{
  return ispunct_l (wc, locale);
}

int
iswspace_l (wint_t wc, locale_t locale)
{
  return isspace_l (wc, locale);
}

int
iswxdigit_l (wint_t wc, locale_t locale)
{
  return isxdigit_l (wc, locale);
}

wint_t
towlower_l (wint_t wc, locale_t locale)
{
  return tolower_l (wc, locale);
}

wint_t
towupper_l (wint_t wc, locale_t locale)
{
  return toupper_l (wc, locale);
}
