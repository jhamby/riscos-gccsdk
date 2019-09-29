/* Copyright (c) 2019 UnixLib Developers
 */

#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <errno.h>

struct _locale {
  struct lconv lc;
};

/* This is supposed to be per-thread.  */
static locale_t current_locale;

locale_t uselocale(locale_t newloc)
{
  locale_t old_locale = current_locale;

  if (!newloc)
    return old_locale;

  current_locale = newloc;

  return old_locale;
}

void freelocale(locale_t locobj)
{
  if (locobj)
    free(locobj);
}

locale_t newlocale(int category_mask, const char *locale,
                          locale_t base)
{
  locale_t loc = (locale_t)malloc(sizeof(*loc));
  if (!loc) {
    __set_errno (ENOMEM);
    return 0;
  }

  loc->lc = *localeconv();

  return loc;
}
