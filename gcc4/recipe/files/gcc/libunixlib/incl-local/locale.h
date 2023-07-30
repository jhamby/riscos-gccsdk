/* Internal UnixLib locale.h
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#ifndef __LOCALE_H
#include_next <locale.h>
#endif

#if !defined(__INTERNAL_LOCALE_H) && defined(__LOCALE_H)
#define	__INTERNAL_LOCALE_H

__BEGIN_DECLS

#ifndef __TARGET_SCL__

struct _locale {
  /* Map from locale category to territory number. C locale is -1. */
  int locale_territory[LC_ALL];

  /* Character type LUTs: EOF + one entry per character
   *
   * EOF is defined as -1, so character entries start at offset 1.
   */
  short ctype_upper[257];
  short ctype_lower[257];
  unsigned char ctype[257];

  /* Set to 1 if the locale information has changed since the last time
   * the lconv structure was updated. */
  int lc_needs_refresh;
  /* Numeric formatting information for locale */
  struct lconv lc;
};

extern struct _locale __locale_global;

extern void __build_ctype_tables (locale_t locobj, int __territory) __THROW;

extern char *__setlocale_l (locale_t locobj, int __category,
                            const char *__locale) __THROW;

void __localeconv_lconv_init (struct lconv *lc);
void __localeconv_lconv_fini (struct lconv *lc);
void __localeconv_l (locale_t locobj);

#endif

__END_DECLS

#endif
