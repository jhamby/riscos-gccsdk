/* locale.h standard header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef __LOCALE_H
#define __LOCALE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Locale information types.  */

/* String collation (functions 'strcoll' and 'strxfrm').  */
#define LC_COLLATE 1

/* Classification and conversion of characters, multibyte and
   wide characters.  */
#define LC_CTYPE 2

/* Formatting of monetary values.  */
#define LC_MONETARY 4

/* Formatting of numeric values that are not monetary. */
#define LC_NUMERIC 8

/* Formatting of data and time values.  */
#define LC_TIME 16

/* Entire locale.  */
#define LC_ALL 31

/* Sets the current locale for category 'category' to 'locale'.

   If 'category' is 'LC_ALL', this specifies the locale for
   all purposes. The other possible values of 'category' specify
   an individual purpose.  */
extern char *setlocale (int __category, const char *__locale);

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

/* Set the lconv structure with the current locale settings.  */
extern struct lconv *localeconv (void);

#ifdef __cplusplus
}
#endif

#endif
