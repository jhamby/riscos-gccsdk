/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

/* Character localisation support. Written by Nick Burrett, 20 July 1997.  */

#include <locale.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unixlib/os.h>
#include <swis.h>

/* #define DEBUG */

int __setlocale_called = 1;

static int read_symbol (int reason_code, int territory)
{
  int regs[10];

  regs[0] = territory;
  regs[1] = reason_code;
  __os_swi (Territory_ReadSymbols, regs);
  return regs[0];
}

static void read_byte_list (int reason_code, char **grouping, int territory)
{
  char *byte_list, *new_grouping;
  char temp[128], *temp1;
#ifdef DEBUG
  char *bl, test[16];
#endif
  int regs[10];

  byte_list = (char *)read_symbol (reason_code, territory);
#ifdef DEBUG
  /* Bytelist test.  */
  test[0] = 1; test[1] = 4; test[2] = 8; test[3] = 17; test[4] = 32;
  test[5] = 255;
  bl = test;
  byte_list = test;
#endif
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
#ifdef DEBUG
  {
    int x;

    printf ("localeconv(): byte_list = ");
    for (x = 0; x <= 10; x++)
      printf ("%d, ", bl[x]);
    printf ("\ntemp = '%s'\n", temp);
  }
#endif

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

/* Defined by POSIX as not threadsafe */
struct lconv *localeconv (void)
{
  static struct lconv lc = { NULL, NULL, NULL, NULL, NULL,
			     NULL, NULL, NULL, NULL, NULL,
			     0, 0, 0, 0, 0, 0, 0, 0 };
  int numeric, monetary;

  /* If setlocale has not been called since the last call to
     localeconv, then the lconv structure will be the same.  */
  if (!__setlocale_called)
    return &lc;

  __setlocale_called = 0;

  numeric = __locale_territory[LC_NUMERIC];
  monetary = __locale_territory[LC_MONETARY];

  /* See the PRMs regarding SWI Territory_ReadSymbols for the
     meanings of the following numbers.  */
  if (numeric == -1)
    {
      /* We're using the 'C' locale.  */
      free (lc.decimal_point);
      lc.decimal_point = strdup (".");
      free (lc.thousands_sep);
      lc.thousands_sep = strdup ("");
      free (lc.grouping);
      lc.grouping = strdup ("");
    }
  else
    {
      lc.decimal_point = (char *) read_symbol (0, numeric);
      lc.thousands_sep = (char *) read_symbol (1, numeric);
      read_byte_list (2, &lc.grouping, numeric);
    }
  if (monetary == -1)
    {
      /* We using the 'C' locale.  Empty strings and CHAR_MAX means
	 that these fields are unspecified.  */
      free(lc.mon_decimal_point);
      lc.mon_decimal_point = strdup ("");
      free(lc.mon_thousands_sep);
      lc.mon_thousands_sep = strdup ("");
      free(lc.mon_grouping);
      lc.mon_grouping = strdup ("");
      lc.int_frac_digits = CHAR_MAX;
      lc.frac_digits = CHAR_MAX;
      free (lc.currency_symbol);
      lc.currency_symbol = strdup ("");
      free (lc.int_curr_symbol);
      lc.int_curr_symbol = strdup ("");
      lc.p_cs_precedes = CHAR_MAX;
      lc.n_cs_precedes = CHAR_MAX;
      lc.p_sep_by_space = CHAR_MAX;
      lc.n_sep_by_space = CHAR_MAX;
      free (lc.positive_sign);
      lc.positive_sign = strdup ("");
      free (lc.negative_sign);
      lc.negative_sign = strdup( "");
      lc.p_sign_posn = CHAR_MAX;
      lc.n_sign_posn = CHAR_MAX;
    }
  else
    {
      lc.int_curr_symbol = (char *)read_symbol (3, monetary);
      lc.currency_symbol = (char *)read_symbol (4, monetary);
      lc.mon_decimal_point = (char *)read_symbol (5, monetary);
      lc.mon_thousands_sep = (char *)read_symbol (6, monetary);
      read_byte_list (7, &lc.mon_grouping, monetary);
      lc.positive_sign = (char *)read_symbol (8, monetary);
      lc.negative_sign = (char *)read_symbol (9, monetary);
      lc.int_frac_digits = (char)read_symbol (10, monetary);
      lc.frac_digits = (char)read_symbol (11, monetary);
      lc.p_cs_precedes = (char)read_symbol (12, monetary);
      lc.p_sep_by_space = (char)read_symbol (13, monetary);
      lc.n_cs_precedes = (char)read_symbol (14, monetary);
      lc.n_sep_by_space = (char)read_symbol (15, monetary);
      lc.p_sign_posn = (char)read_symbol (16, monetary);
      lc.n_sign_posn = (char)read_symbol (17, monetary);
    }
  return &lc;
}
