/*
 * ANSI Standard 4.4: Localisation <locale.h>
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef	__LOCALE_H
#define	__LOCALE_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

/* Locale information types.
   If you change these values, then you better fix setlocale().  */

/* String collation (functions 'strcoll' and 'strxfrm').  */
#define LC_COLLATE 0
/* Classification and conversion of characters, multibyte and
   wide characters.  */
#define LC_CTYPE 1
/* Localisable natural-language messages.  */
#define LC_MESSAGES 2
/* Formatting of monetary values.  */
#define LC_MONETARY 3
/* Formatting of numeric values that are not monetary. */
#define LC_NUMERIC 4
/* Formatting of data and time values.  */
#define LC_TIME 5
/* Entire locale.  */
#define LC_ALL 6

struct lconv
{
  /* Decimal-point separators used in formatting non-monetary quantities.  */
  char *decimal_point;
  /* Separators used to delimit groups of digits to the left of the
     decimal point in formatting non-monetary quantities.  */
  char *thousands_sep;
  /* A string that specifies how to group the digits to the left
     of the decimal point for non-monetary quantities.  */
  char *grouping;
  /* The international currency symbol for the selected locale.  */
  char *int_curr_symbol;
  /* The local currency symbol for the selected locale.  */
  char *currency_symbol;
  /* Decimal-point separators used in formatting monetary quantities.  */
  char *mon_decimal_point;
  /* Separators used to delimit groups of digits to the left of the
     decimal point in formatting monetary quantities.  */
  char *mon_thousands_sep;
  /* A string that specifies how to group the digits to the left
     of the decimal point for monetary quantities.  */
  char *mon_grouping;
  /* String used to indicate positive (or zero) monetary quantities.  */
  char *positive_sign;
  /* String used to indicate negative monetary quantities.  */
  char *negative_sign;
  /* Small integers indicating how many fractional digits should
     be displayed in a monetary value in international (int_frac_digits)
     and local formats (frac_digits).  */
  char int_frac_digits;
  char frac_digits;
  /* Set to 1 is the 'currency_symbol' string should precede the
     value of a monetary amount, 0 if the string should follow the value.  */
  char p_cs_precedes;
  /* Set to 1 if a space should appear between the 'currency_symbol'
     string and the amount, 0 if no space should appear.  */
  char p_sep_by_space;
  /* Set to 1 is the 'currency_symbol' string should precede the
     value of a monetary amount, 0 if the string should follow the value.  */
  char n_cs_precedes;
  /* Set to 1 if a space should appear between the 'currency_symbol'
     string and the amount, 0 if no space should appear.  */
  char n_sep_by_space;
  /* Indicate how to position the sign for non-negative monetary quantities.  */
  char p_sign_posn;
  /* Indicate how to position the sign for negative monetary quantities.  */
  char n_sign_posn;
};

__BEGIN_NAMESPACE_STD

/* Sets the current locale for category 'category' to 'locale'.

   If 'category' is 'LC_ALL', this specifies the locale for
   all purposes. The other possible values of 'category' specify
   an individual purpose.  */
extern char *setlocale (int __category, const char *__locale) __THROW;

/* Set the lconv structure with the current locale settings.  */
extern struct lconv *localeconv (void) __THROW;

__END_NAMESPACE_STD

__END_DECLS

#endif
