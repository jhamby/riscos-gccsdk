/* Copyright (c) 2019 UnixLib Developers
 */

#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <internal/unix.h>

/* This is supposed to be per-thread.  */
static locale_t current_locale = LC_GLOBAL_LOCALE;

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
  if (locobj) {
    __localeconv_lconv_fini(&locobj->lc);
    free(locobj);
  }
}

locale_t newlocale(int category_mask, const char *locale,
                          locale_t base)
{
  struct _locale tmp;

  PTHREAD_UNSAFE

  if ((category_mask & ~LC_ALL_MASK) || locale == NULL) {
    __set_errno (EINVAL);
    return 0;
  }

  /* The locale provided will be verified by __setlocale_l() */

  /* Prepare the temporary locale we will modify */
  if (base != 0 && base != LC_GLOBAL_LOCALE) {
    memcpy(&tmp, base, sizeof(tmp));
  } else {
    if (base == LC_GLOBAL_LOCALE) {
      /* Undefined: be helpful to client */
      memcpy(&tmp, &__locale_global, sizeof(tmp));
    } else {
      /* Initialise to C locale */
      __build_ctype_tables(&tmp, -2);
    }
  }

  /* Now, apply the requested locale to each of the categories in the mask */
  if (category_mask == LC_ALL_MASK) {
    /* Special-case LC_ALL to handle the encoded locale string */
    if (__setlocale_l(&tmp, LC_ALL, locale) == NULL) {
      return 0;
    }
  } else {
    int category = 0, mask = category_mask;
    while (mask != 0) {
      if (mask & 1) {
        if (__setlocale_l(&tmp, category, locale) == NULL) {
          return 0;
        }
      }
      category++;
      mask >>= 1;
    }
  }

  /* Allocate the result, if necessary */
  if (base == 0 || base == LC_GLOBAL_LOCALE) {
    base = (locale_t)malloc(sizeof(*base));
    if (!base) {
      __set_errno (ENOMEM);
      return 0;
    }
  }
  /* Fill it in */
  memcpy(base, &tmp, sizeof(tmp));

  /* Mark the lconv data stale */
  base->lc_needs_refresh = 1;

  return base;
}

locale_t duplocale(locale_t locobj)
{
  locale_t loc;

  loc = (locale_t) malloc(sizeof(*loc));
  if (loc == NULL) {
    __set_errno (ENOMEM);
    return 0;
  }

  if (locobj == LC_GLOBAL_LOCALE) {
    memcpy(loc, &__locale_global, sizeof(*loc));
  } else {
    memcpy(loc, locobj, sizeof(*loc));
  }

  /* Invalidate lconv in the copy */
  __localeconv_lconv_init(&loc->lc);
  loc->lc_needs_refresh = 1;

  return loc;
}
