/*
Routines to output various things.

Copyright (C) 1991-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         Peter Gerwinski <peter@gerwinski.de>
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

static void
_p_write_to_buf (FDR File, const char *ptr, size_t size)
{
  size_t a;
  a = File->BufSize - File->BufPos;
  if (size < a) a = size;
  if (a > 0)
    {
      _p_move (ptr, File->BufPtr + File->BufPos, a);
      File->BufPos += a;
      ptr += a;
      size -= a;
    }
  if (size == 0) return;
  if (File->Flags & READ_WRITE_STRING_MASK)
    {
      if (File->Flags & TRUNCATE_STRING_MASK)
        return;
      else
        IOERROR (582,); /* string capacity exceeded in `WriteStr' */
    }
  if (_p_inoutres) return;
  _p_internal_write (File->BufPtr, File->BufPos, File);
  if (size <= File->BufSize)
    {
      _p_move (ptr, File->BufPtr, size);
      File->BufPos = size;
    }
  else
    {
      _p_internal_write (ptr, size, File);
      File->BufPos = 0;
    }
}

void
_p_write_flush (FDR File)
{
  if (_p_inoutres) return;
  if (File->BufPos != 0)
    _p_internal_write (File->BufPtr, File->BufPos, File);
  _p_clearbuffer (File);
  _p_flushbuffer (File);
}

/* pad with spaces */
#define PADSIZE 32
static char const blanks[PADSIZE] =
{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

static void
_p_write_pad (FDR File, int count)
{
  register int i;
  for (i = count; i > 0 ; i -= PADSIZE)
    _p_write_to_buf (File, blanks, (i >= PADSIZE) ? PADSIZE : i);
}

static void
_p_write_padded (FDR File, const char *buf, int length, int width, int clip)
{
  int pad_left = 0, pad_right = 0;
  if (width != _p_low_integer)
    {
      int abs_width, pad;
      abs_width = (width >= 0) ? width : - width;
      if (length > abs_width)
        {
          pad = 0;
          if (clip) length = abs_width;
        }
      else
        pad = abs_width - length;
      if (width >= 0)
        pad_left = pad;
      else
        {
          switch (File->Flags & NEG_WIDTH_MASKS)
            {
              case NEG_WIDTH_ERROR_MASK:  IOERROR (580,); /* fixed field width cannot be negative */
              case NEG_WIDTH_LEFT_MASK:   pad_right = pad;
                                          break;
              case NEG_WIDTH_CENTER_MASK: pad_left = pad / 2;
                                          pad_right = pad - pad_left;
            }
        }
    }
  _p_write_pad (File, pad_left);
  _p_write_to_buf (File, buf, length);
  _p_write_pad (File, pad_right);
}

/* Sufficient width to hold a signed long long in decimal representation */
#define MAX_LONG_WIDTH (sizeof (long long) * 64/*BITS_PER_UNIT*/ / 3 + 2)

#define DEFWRITEINT(fnname, type, conv_fn)          \
void                                                \
fnname (FDR File, type num, int width)              \
{                                                   \
  char buf[MAX_LONG_WIDTH], *buf_begin;             \
  int negative = num < 0;                           \
  if (negative) num = - num;                        \
  buf_begin = conv_fn (num, buf + MAX_LONG_WIDTH);  \
  if (negative) *(--buf_begin) = '-';               \
  _p_write_padded (File, buf_begin, buf + MAX_LONG_WIDTH - buf_begin, width, 0); \
}
DEFWRITEINT (_p_write_integer, signed int, _p_card_to_decimal)
DEFWRITEINT (_p_write_longint, signed long long, _p_longcard_to_decimal)

#define DEFWRITEUINT(fnname, type, conv_fn)         \
void                                                \
fnname (FDR File, type num, int width)              \
{                                                   \
  char buf[MAX_LONG_WIDTH], *buf_begin;             \
  buf_begin = conv_fn (num, buf + MAX_LONG_WIDTH);  \
  _p_write_padded (File, buf_begin, buf + MAX_LONG_WIDTH - buf_begin, width, 0); \
}
DEFWRITEUINT (_p_write_cardinal, unsigned int, _p_card_to_decimal)
DEFWRITEUINT (_p_write_longcard, unsigned long long, _p_longcard_to_decimal)

void
_p_write_real (FDR File, long double num, int width, int prec)
{
  char *buf;
  int buf_size;
  if (prec < 0 && prec != _p_low_integer)
    IOERROR (581,); /* fixed real fraction field width cannot be negative */
  buf = _p_longreal_to_decimal (num, width, prec,
        width != _p_low_integer,
        (File->Flags & REAL_NOBLANK_MASK) == 0,
        (File->Flags & REAL_CAPITAL_EXP_MASK) != 0, &buf_size);
  _p_write_padded (File, buf, _p_strlen (buf), width, 0);
  if (buf_size)
    _p_dispose (buf);
}

void
_p_write_char (FDR File, char ch, int width)
{
  _p_write_padded (File, &ch, sizeof (ch), width, 0);
}

void
_p_write_boolean (FDR File, int b, int width)
{
  const char *str_val = b ? TRUE_str : FALSE_str;
  _p_write_padded (File, str_val, _p_strlen (str_val), width, 1);
}

void
_p_write_enum (FDR File, char **IDs, int IDCount, int v, int width)
{
  const char *str = (v < 0 || v >= IDCount) ? "invalid enumeration value" : IDs [v];
  _p_write_padded (File, str, strlen (str), width, 0);
}

void
_p_write_string (FDR File, char *s, int length, int width)
{
  if (s == NULL)
    length = 0;
  else if (length < 0)  /* CString */
    length = strlen (s);
  _p_write_padded (File, s, length, width, File->Flags & CLIP_STRING_MASK);
}

void
_p_writeln (FDR File)
{
  char newline = NEWLINE;
  _p_write_to_buf (File, &newline, sizeof (newline));
}

void
_p_write_init (FDR File, int Flags)
{
  _p_ok_WRITE (File);
  File->BufSize = FILE_BUFSIZE;
  File->BufPos = 0;
  File->Flags = Flags;
}

void
_p_writestr_init (FDR File, char *s, int Capacity, int Flags)
{
  File->BufPtr = s;
  File->BufSize = Capacity;
  File->BufPos = 0;
  File->Flags = Flags;
}

int
_p_writestr_getlength (FDR File)
{
  return File->BufPos;
}

void
_p_writestr_one_real (char *s1, int *long_curlen, int Capacity, long double num, int width, int prec)
{
  struct Fdr TempFile; /* This is no real file, be careful what you do with it. Don't call initfdr(). ;*/
  if (_p_inoutres) return;
  _p_writestr_init (&TempFile, s1, Capacity, READ_WRITE_STRING_MASK | TRUNCATE_STRING_MASK);
  _p_write_real (&TempFile, num, width, prec);
  *long_curlen = _p_writestr_getlength (&TempFile);
}
