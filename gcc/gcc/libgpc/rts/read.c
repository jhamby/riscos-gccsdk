/*
Routines to read various things from files.

Copyright (C) 1991-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         Frank Heckenbach <frank@pascal.gnu.de>

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

As a special exception, if you link this file with files compiled
with a GNU compiler to produce an executable, this does not cause
the resulting executable to be covered by the GNU General Public
License. This exception does not however invalidate any other
reasons why the executable file might be covered by the GNU General
Public License.
*/

#include "rts.h"
#include "fdr.h"

#define IsSpaceNl(ch) ((ch) == ' ' || (ch) == '\t' || (ch) == '\n')
#define IsDigit(ch) ((ch) >= '0' && (ch) <= '9')

int
_p_internal_getc (FDR File)
{
  if (!(File->Flags & READ_WRITE_STRING_MASK))
    {
      if (tst_EOF (File)) return EOF;
      if (File->BufPos >= File->BufSize)
        _p_read_buffer (File);
    }
  if (_p_inoutres == 0)
    {
      clr_EOLN (File);
      if (File->BufPos < File->BufSize)
        {
          unsigned char ch = File->BufPtr [File->BufPos++];
          if (tst_TXT (File) && ch == NEWLINE)
            {
              clr_LGET (File);
              set_EOLN (File);
              set_EOFOK (File);
              m_FILBUF (File) = ch = ' ';
            }
          return ch;
        }
      set_EOF (File);
      set_EOLN (File);
    }
  return EOF;
}

static int
_p_direct_getc (FDR File)
{
  /* If buffer is undefined, read in new contents */
  if (!tst_LGET (File))
    {
      set_LGET (File);
      return m_FILBUF (File);
    }
  return _p_internal_getc (File);
}

static int
_p_direct_getc_checkeof (FDR File)
{
  int ch = _p_direct_getc (File);
  if (_p_ok_EOF (File))
    return ch;
  else
    return EOF;
}

static void
_p_ungetch (FDR File, int ch)
{
  if (ch != EOF && !tst_EOLN (File))
    {
      if (File->BufPos == 0) _p_internal_error (910); /* read buffer underflow */ \
      File->BufPos--;
      /* Don't do this for strings, because it's not necessary, and the
         string might be constant */
      if (!(File->Flags & READ_WRITE_STRING_MASK))
        File->BufPtr [File->BufPos] = ch;
    }
}

static inline int
TestDigit (int ch, int *digit, int base)
{
  *digit = (ch >= '0' && ch <= '9') ? ch - '0'
         : (ch >= 'A' && ch <= 'Z') ? ch - 'A' + 10
         : (ch >= 'a' && ch <= 'z') ? ch - 'a' + 10
         : base;
  return *digit < base;
}

/* read an integer number
   Actually min and max are unsigned if check == UnsignedRangeCheck */
signed long long
_p_readi_check (FDR File, TRangeCheck check, signed long long min, signed long long max)
{
  int negative;
  int ch;
  unsigned long long num,          /* Absolute value of the number read */
                     u_min, u_max; /* bounds for the absolute value */
  int base = 10;
  int base_changed = 0;
  int digit;

  if (_p_inoutres) return 0;

  negative = FALSE;
  do
    {
      ch = _p_direct_getc_checkeof (File);
      if (_p_inoutres) return 0;
    }
  while (IsSpaceNl (ch));
  if (! (TestDigit (ch, &digit, base) || ch == '+' || ch == '-' || (ch == '$' && (File->Flags & INT_READ_HEX_MASK))))
    IOERROR (552, 0); /* sign or digit expected */
  else
    {
      if (ch=='+' || ch=='-')
        {
          if (ch == '-')
            negative = TRUE;
          ch = _p_direct_getc_checkeof (File);
          if (_p_inoutres) return 0;
          if (! (TestDigit (ch, &digit, base) || (ch == '$' && (File->Flags & INT_READ_HEX_MASK))))
            IOERROR (551, 0); /* digit expected after sign */
        }
    }

  /* Compute bounds for absolute value, depending on the actual sign */
  u_min = min;
  u_max = max;

  if (check == UnsignedRangeCheck && negative)
    {
      if (u_min == 0)
        u_max = 0;
      else
        IOERROR (553, 0); /* Overflow while reading integer */
    }

  if (check == SignedRangeCheck)
    {
      if (negative)
        {
          if (min > 0) IOERROR (553, 0); /* Overflow while reading integer */
          u_max = - min;
          u_min = (max > 0) ? 0 : - max;
        }
      else
        {
          if (min < 0) u_min = 0;
          if (max < 0) IOERROR (553, 0); /* Overflow while reading integer */
        }
    }

  /* Check for `$' hex base specifier */
  if (ch == '$' && (File->Flags & INT_READ_HEX_MASK))
    {
      base = 0x10;
      base_changed = 1;
      ch = _p_direct_getc_checkeof (File);
      if (_p_inoutres) return 0;
      if (!TestDigit (ch, &digit, base))
        IOERROR (557, 0); /* digit expected after `$'' in integer constant */
    }

  /* Now the 'ch' contains the first digit. Get the integer */

  num = 0;

  do
    {
      if (check != NoRangeCheck &&
          num > (((u_max < 36 && !base_changed && (File->Flags & INT_READ_BASE_SPEC_MASK))
                  ? 36 : u_max) - digit) / base)
        IOERROR (553, 0); /* Overflow while reading integer */

      num = num * base + digit;
      ch = _p_direct_getc (File);

      /* Check for `n#' base specifier */
      if (ch == '#' && (File->Flags & INT_READ_BASE_SPEC_MASK))
        {
          if (base_changed)
            IOERROR (559, 0); /* Only one base specifier allowed in integer constant */

          if (num < 2 || num > 36)
            IOERROR (560, 0); /* Base out of range (2..36) */

          base = num;
          base_changed = 1;
          num = 0;

          ch = _p_direct_getc_checkeof (File);
          if (_p_inoutres) return 0;
          if (!TestDigit (ch, &digit, base))
            IOERROR (558, 0); /* Digit expected after `#' in integer constant */
        }
    }
  while (TestDigit (ch, &digit, base));

  if (check != NoRangeCheck && (num < u_min || num > u_max))
    IOERROR (553, 0); /* Overflow while reading integer */

  if ((File->Flags & NUM_READ_CHK_WHITE_MASK) && !IsSpaceNl (ch))
    IOERROR (561, 0); /* Invalid digit */

  _p_ungetch (File, ch);

  if ((File->Flags & VAL_MASK) && File->BufPos < File->BufSize)
    {
      File->BufPos++;
      IOERROR (565, 0); /* extra characters after number in `Val'' */
    }

  return negative ? -num : num;
}

signed long long
_p_readi (FDR File)
{
  return _p_readi_check (File, NoRangeCheck, 0ll, 0ll);
}

/* check if two real numbers are approximately equal */
static int
_p_realeq (long double X, long double Y)
{
  long double tmp = 1.0e-6 * ((X >= 0) ? X : -X);
  return X-Y <= tmp && Y-X <= tmp;
}

static void
_p_check_real_overunderflow (long double tmp, long double p)
{
  if (_p_inoutres) return;
  if (p == 0.0 && tmp != 0.0)
    IOERROR (564,); /* underflow while reading real number */
  if ((tmp < -1.0 || tmp > 1.0) && !_p_realeq (tmp, p))
    IOERROR (563,); /* overflow while reading real number */
}

long double
_p_read_longreal (FDR File)
{
  int require_fractional = 0;
  int negative = FALSE;
  int expon = 0, lastexp;
  int enegative = FALSE;
  int ch, i;
  long double val = 0.0, lastval, frac = 1.0;

  if (_p_inoutres) return 0.0;

  ch = _p_direct_getc_checkeof (File);
  if (_p_inoutres) return 0.0;

  while (IsSpaceNl (ch))
    {
      ch = _p_direct_getc_checkeof (File);
      if (_p_inoutres) return 0.0;
    }
  if (!(IsDigit (ch) || ch == '+' || ch == '-' || (ch == '.' && !(File->Flags & REAL_READ_ISO7185_ONLY_MASK))))
    IOERROR (552, 0.0); /* Sign or digit expected */
  else
    {
      if (ch == '+' || ch == '-')
        {
          if (ch == '-')
            negative = TRUE;
          ch = _p_direct_getc_checkeof (File);
          if (_p_inoutres) return 0.0;

          /* Skip spaces between sign and digit (or '.') */
          while (IsSpaceNl (ch))
            {
              ch = _p_direct_getc_checkeof (File);
              if (_p_inoutres) return 0.0;
            }
        }
    }

  if (! (IsDigit (ch) || (ch == '.' && !(File->Flags & REAL_READ_ISO7185_ONLY_MASK))))
    IOERROR ((File->Flags & REAL_READ_ISO7185_ONLY_MASK) ? 551 : 562, 0.0);
     /* Digit expected after sign; Digit or `.' expected after sign */

  require_fractional = ((File->Flags & REAL_READ_ISO7185_ONLY_MASK) || !IsDigit (ch));

  /* Read the mantissa. ch is now a digit (or '.') */
  while (IsDigit (ch))
    {
      lastval = val;
      val = 10.0 * val + (ch - '0');
      if (!_p_realeq ((val - (ch - '0')) / 10.0, lastval))
        IOERROR (563, 0.0); /* Overflow while reading real number */
      ch = _p_direct_getc (File);
    }

  if (ch == '.')
    {
      /* Read the fractional part */
      ch = _p_direct_getc (File);

      if (require_fractional && !IsDigit (ch))
        IOERROR (554, 0.0); /* Digit expected after decimal point */

      while (IsDigit (ch))
        {
          frac /= 10.0;
          val += frac * (ch - '0');
          ch = _p_direct_getc (File);
        }
    }

  /* read the exponent */
  if (ch == 'e' || ch == 'E') {
    ch = _p_direct_getc_checkeof (File);
    if (_p_inoutres) return 0.0;
    if (ch == '+' || ch == '-') {
      if (ch == '-')
        enegative = TRUE;
      ch = _p_direct_getc_checkeof (File);
      if (_p_inoutres) return 0.0;
    }

    if (!IsDigit (ch))
      IOERROR (555, 0.0); /* Digit expected while reading exponent */

    while (IsDigit (ch)) {
      lastexp = expon;
      expon = 10 * expon + (ch - '0');
      if ((expon - (ch - '0')) / 10 != lastexp)
        IOERROR (556, 0.0); /* Exponent out of range */
      ch = _p_direct_getc (File);
    }

    if (enegative)
      {
        frac = 1.0;
        for (i = 1; i <= expon; i++)
          frac /= 10.0;

        if (frac == 0.0)
          IOERROR (556, 0.0); /* Exponent out of range */

        val *= frac;
      }
    else
      for (i = 1; i<= expon; i++)
      {
        lastval = val;
        val = 10.0 * val;
        if (!_p_realeq (val / 10.0, lastval))
          IOERROR (556, 0.0); /* Exponent out of range */
      }

  }

  if ((File->Flags & NUM_READ_CHK_WHITE_MASK) && !IsSpaceNl (ch))
    IOERROR (561, 0.0); /* invalid digit */

  _p_ungetch (File, ch);

  if ((File->Flags & VAL_MASK) && File->BufPos < File->BufSize)
    {
      File->BufPos++;
      IOERROR (565, 0.0); /* extra characters after number in `Val'' */
    }

  return negative ? -val : val;
}

float
_p_read_shortreal (FDR File)
{
  long double tmp = _p_read_longreal (File);
  volatile float p = (float) tmp;
  _p_check_real_overunderflow (tmp, (long double) p);
  return _p_inoutres ? 0.0 : p;
}

double
_p_read_real (FDR File)
{
  long double tmp = _p_read_longreal (File);
  volatile double p = (double) tmp;
  _p_check_real_overunderflow (tmp, (long double) p);
  return _p_inoutres ? 0.0 : p;
}

char
_p_read_char (FDR File)
{
  if (_p_inoutres)
    return ' ';
  else
    return _p_direct_getc_checkeof (File);
}

static char *
_p_read_word (FDR File)
{
  int ch, length = 0, size = 16;
  char *buf;
  if (_p_inoutres)
    return NULL;
  buf = _p_malloc (size);
  do
    {
      ch = _p_direct_getc_checkeof (File);
      if (_p_inoutres) return NULL;
    }
  while (IsSpaceNl (ch));
  do
    {
      buf [length++] = ch;
      if (length >= size)
        _p_gpc_reallocmem ((void **) &buf, size *= 2);
      ch = _p_direct_getc (File);
    }
  while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')
         || (ch >= '0' && ch <= '9') || ch == '_');
  _p_ungetch (File, ch);
  buf [length] = 0;
  return buf;
}

Boolean
_p_read_boolean (FDR File)
{
  char *v;
  Boolean Result;
  v = _p_read_word (File);
  if (_p_inoutres)
    {
      _p_free (v);
      return FALSE;
    }
  if (!_p_strcasecmp (v, FALSE_str))
    Result = FALSE;
  else if (!_p_strcasecmp (v, TRUE_str))
    Result = TRUE;
  else
    Result = 2;
  _p_free (v);
  if (Result == 2)
    IOERROR (566, FALSE); /* invalid Boolean value read */
  return Result;
}

int
_p_read_enum (FDR File, char **IDs, int IDCount)
{
  char *v;
  int Result = 0;
  v = _p_read_word (File);
  if (_p_inoutres)
    {
      _p_free (v);
      return 0;
    }
  while (Result < IDCount && _p_strcasecmp (v, IDs [Result]))
    Result++;
  _p_free (v);
  if (Result >= IDCount)
    IOERROR (567, 0); /* invalid enumaration value read */
  return Result;
}

/* read a string up to the max length or newline, whichever comes first.
   The number of characters read is returned. */
int
_p_read_string (FDR File, char *str, int maxlen)
{
  int length = 0;
  if (_p_inoutres) return 0;
  if (maxlen < 0)
    _p_internal_error (907); /* String length cannot be negative */
  /* If EOLn (File) is on, nothing is read and length is left zero. */
  if (!tst_EOLN (File))
    {
      int ch = _p_direct_getc (File);
      while (!tst_EOLN (File) && ch != EOF && length < maxlen)
        {
          *str++ = ch;
          length++;
          ch = _p_direct_getc (File);
        }
      _p_ungetch (File, ch);
    }
  return length;
}

void
_p_readln (FDR File)
{
  if (_p_inoutres) return;
  while (!tst_EOF (File) && !tst_EOLN (File))
    _p_getbyte (File);
  /* Now EOLn is not true, because we just read it off */
  clr_EOLN (File);
  set_LGET (File);
  set_EOFOK (File);
}

void
_p_read_init (FDR File, int Flags)
{
  File->Flags = Flags;
  _p_ok_READ (File);
}

void
_p_readstr_init (FDR File, char *s, int Length, int Flags)
{
  File->BufPtr = s;
  File->BufSize = Length;
  File->BufPos = 0;
  File->Flags = Flags;
  m_FILSTA (File) = 0;
  clr_EOF (File);
  clr_EOFOK (File);
  clr_EOLN (File);
  set_LGET (File);
  set_TXT (File);
  SET_STATUS (File, FiRONLY);
  if (File->BufPos >= File->BufSize)
    {
      set_EOF (File);
      set_EOLN (File);
    }
}

void
_p_val_init (FDR File, char *s, int Length, int Flags)
{
  _p_start_temp_io_error ();
  _p_readstr_init (File, s, Length, Flags);
}

int
_p_get_val_return_value (FDR File)
{
  return _p_end_temp_io_error () ? File->BufPos + !!tst_EOF (File) : 0;
}
