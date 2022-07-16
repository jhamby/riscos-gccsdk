/* localeconv ()
 * Written by Nick Burrett, 20 July 1997.
 * Copyright (c) 1997-2011 UnixLib Developers
 */

#include <locale.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <swis.h>

#include <internal/os.h>

/* #define DEBUG */

static int
read_symbol (int reason_code, int territory)
{
  int regs[10];

  regs[0] = territory;
  regs[1] = reason_code;
  __os_swi (Territory_ReadSymbols, regs);
  return regs[0];
}

static void
read_byte_list (int reason_code, char **grouping, int territory)
{
  char *byte_list, *new_grouping;
  char temp[128], *temp1;
  int regs[10];

  byte_list = (char *)read_symbol (reason_code, territory);
  /* Build a grouping string.  */
  temp[0] = '\0';
  temp1 = temp;
  /* We should never overflow the buffer, but if we do then just ignore any
     further numbers in the byte_list. Each iteration puts up to 4 characters
     into the buffer.  */
  while (*byte_list != 0 && *byte_list != 255 && temp1 < &temp[sizeof(temp)-4])
    {
      /* Convert byte to ascii inline. Do not use sprintf since that pulls
	 in stdio in minimal UnixLib programs.  */

      regs[0] = *byte_list++;
      *temp1++ = ';';
      regs[1] = (int)temp1;
      regs[2] = (int)(&temp[sizeof (temp) - 4] - temp1);
      __os_swi (OS_BinaryToDecimal, regs);
      temp1 += regs[2];
    }
  *temp1 = '\0';

  if (*grouping)
    new_grouping = realloc (*grouping, strlen (temp) + 1);
  else
    new_grouping = malloc (strlen (temp) + 1);

  if (new_grouping == NULL)
    {
      if (*grouping)
	free (*grouping);
    }
  else
    /* Remove the first semi-colon.  */
    strcpy (new_grouping, temp + 1);
  *grouping = new_grouping;
}

void
__localeconv_lconv_init (struct lconv *lc)
{
  memset(lc, 0, sizeof(*lc));
  lc->int_frac_digits = lc->frac_digits = lc->p_cs_precedes =
  lc->p_sep_by_space = lc->n_cs_precedes = lc->n_sep_by_space =
  lc->p_sign_posn = lc->n_sign_posn = CHAR_MAX;
}

void
__localeconv_lconv_fini (struct lconv *lc)
{
  free(lc->decimal_point);
  free(lc->thousands_sep);
  free(lc->grouping);
  free(lc->int_curr_symbol);
  free(lc->currency_symbol);
  free(lc->mon_decimal_point);
  free(lc->mon_thousands_sep);
  free(lc->mon_grouping);
  free(lc->positive_sign);
  free(lc->negative_sign);
}

void
__localeconv_l (locale_t locobj)
{
  int numeric, monetary;

  /* If setlocale has not been called since the last call to
     localeconv, then the lconv structure will be the same.  */
  if (!locobj->lc_needs_refresh)
    return;

  locobj->lc_needs_refresh = 0;

  numeric = locobj->locale_territory[LC_NUMERIC];
  monetary = locobj->locale_territory[LC_MONETARY];

  /* See the PRMs regarding SWI Territory_ReadSymbols for the
     meanings of the following numbers.  */
  if (numeric == -1)
    {
      /* We're using the 'C' locale.  */
      free (locobj->lc.decimal_point);
      locobj->lc.decimal_point = strdup (".");
      free (locobj->lc.thousands_sep);
      locobj->lc.thousands_sep = strdup ("");
      free (locobj->lc.grouping);
      locobj->lc.grouping = strdup ("");
    }
  else
    {
      free (locobj->lc.decimal_point);
      locobj->lc.decimal_point = strdup ((char *) read_symbol (0, numeric));
      free (locobj->lc.thousands_sep);
      locobj->lc.thousands_sep = strdup ((char *) read_symbol (1, numeric));
      read_byte_list (2, &locobj->lc.grouping, numeric);
    }
  if (monetary == -1)
    {
      /* We using the 'C' locale.  Empty strings and CHAR_MAX means
	 that these fields are unspecified.  */
      free (locobj->lc.mon_decimal_point);
      locobj->lc.mon_decimal_point = strdup ("");
      free (locobj->lc.mon_thousands_sep);
      locobj->lc.mon_thousands_sep = strdup ("");
      free (locobj->lc.mon_grouping);
      locobj->lc.mon_grouping = strdup ("");
      locobj->lc.int_frac_digits = CHAR_MAX;
      locobj->lc.frac_digits = CHAR_MAX;
      free (locobj->lc.currency_symbol);
      locobj->lc.currency_symbol = strdup ("");
      free (locobj->lc.int_curr_symbol);
      locobj->lc.int_curr_symbol = strdup ("");
      locobj->lc.p_cs_precedes = CHAR_MAX;
      locobj->lc.n_cs_precedes = CHAR_MAX;
      locobj->lc.p_sep_by_space = CHAR_MAX;
      locobj->lc.n_sep_by_space = CHAR_MAX;
      free (locobj->lc.positive_sign);
      locobj->lc.positive_sign = strdup ("");
      free (locobj->lc.negative_sign);
      locobj->lc.negative_sign = strdup ("");
      locobj->lc.p_sign_posn = CHAR_MAX;
      locobj->lc.n_sign_posn = CHAR_MAX;
    }
  else
    {
      free (locobj->lc.int_curr_symbol);
      locobj->lc.int_curr_symbol = strdup ((char *)read_symbol (3, monetary));
      free (locobj->lc.currency_symbol);
      locobj->lc.currency_symbol = strdup ((char *)read_symbol (4, monetary));
      free (locobj->lc.mon_decimal_point);
      locobj->lc.mon_decimal_point = strdup ((char *)read_symbol (5, monetary));
      free (locobj->lc.mon_thousands_sep);
      locobj->lc.mon_thousands_sep = strdup ((char *)read_symbol (6, monetary));
      read_byte_list (7, &locobj->lc.mon_grouping, monetary);
      free (locobj->lc.positive_sign);
      locobj->lc.positive_sign = strdup ((char *)read_symbol (8, monetary));
      free (locobj->lc.negative_sign);
      locobj->lc.negative_sign = strdup ((char *)read_symbol (9, monetary));
      locobj->lc.int_frac_digits = (char)read_symbol (10, monetary);
      locobj->lc.frac_digits = (char)read_symbol (11, monetary);
      locobj->lc.p_cs_precedes = (char)read_symbol (12, monetary);
      locobj->lc.p_sep_by_space = (char)read_symbol (13, monetary);
      locobj->lc.n_cs_precedes = (char)read_symbol (14, monetary);
      locobj->lc.n_sep_by_space = (char)read_symbol (15, monetary);
      locobj->lc.p_sign_posn = (char)read_symbol (16, monetary);
      locobj->lc.n_sign_posn = (char)read_symbol (17, monetary);
    }
  return;
}

/* Defined by POSIX as not threadsafe */
struct lconv *
localeconv (void)
{
  __localeconv_l (&__locale_global);

  return &__locale_global.lc;
}
