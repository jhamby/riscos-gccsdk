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

/* Territory number for each locale.  C locale is -1.  */
extern int __locale_territory[LC_ALL + 1];

/* Set to 1 is setlocale has been called since the last call to
   localeconv. localeconv uses this flag to cache the lconv structure.  */
extern int __setlocale_called;

extern void __build_ctype_tables (int __territory) __THROW;

#endif

__END_DECLS

#endif
