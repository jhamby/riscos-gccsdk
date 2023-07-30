/* iswalnum_l(), iswalpha_l(), iswcntrl_l(), iswdigit_l(), iswgraph_l(), towlower_l(),
 * iswprint_l(), iswpunct_l(), iswspace_l(), towupper_l(), iswxdigit_l()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <ctype.h>
#include <locale.h>
#include <wctype.h>

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
