/*
All RTS routines that take varargs.

Copyright (C) 1993-2000 Free Software Foundation, Inc.

Authors: Juki <jtv@hut.fi>
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
#include "varargs.h"

int
_p_string (va_alist)
va_dcl
{
    register va_list p;

    char *s1, *s2 = (char *)NULL;
    int  len1, len2 = 0;
    char c1, c2;

    int code;
    int argument_mask;
    int retval = 0;

    va_start (p);
    code = va_arg (p, int); /* First argument is the opcode */

    argument_mask = va_arg (p, int); /* mask of argument types */
    /* First argument is always a string or char */
    if (argument_mask & P_STR_FIRST_IS_CHAR)
      {
        c1   = va_arg (p, int);
        s1   = &c1;
        len1 = 1;
      }
    else
      {
        /* It's a string */
        s1   = va_arg (p, char *);
        len1 = va_arg (p, int);
      }

    if (code == R_TRIM)
      {
        char *dest = va_arg (p, char *);
        int *dlen  = va_arg (p, int *);

        int inx = _p_trim (s1, len1, dest);
        if (dlen)
          *dlen = inx;

        /* return value not used */
        va_end (p);
        return 0;
      }

    SAVE_RETURN_ADDRESS;
    if (code == R_COPY || code == R_SUBSTR)
      {
        char *dest = va_arg (p, char *);
        int *dlen  = va_arg (p, int *);
        int from   = va_arg (p, int);
        int length = va_arg (p, int);

        int curlen = _p_substr (s1, len1, from, length, dest, code == R_COPY);

        if (dlen)
          *dlen = curlen;

        /* return value not used */
        va_end (p);
        RESTORE_RETURN_ADDRESS;
        return 0;
      }

    /* two string args, either may be a string or char */
    if (argument_mask & P_STR_SECOND_IS_CHAR)
      {
        c2   = va_arg (p, int);
        s2   = &c2;
        len2 = 1;
      }
    else
      {
        /* It's a string */
        s2   = va_arg (p, char *);
        len2 = va_arg (p, int);
      }

    switch (code) {
      case R_EQ:
        retval =  _p_eq (s1, len1, s2, len2); break;
      case R_LT:
        retval =  _p_lt (s1, len1, s2, len2); break;
      case R_GT:
        retval =  _p_lt (s2, len2, s1, len1); break;
      case R_NE:
        retval = !_p_eq (s1, len1, s2, len2); break;
      case R_LE:
        retval = !_p_lt (s2, len2, s1, len1); break;
      case R_GE:
        retval = !_p_lt (s1, len1, s2, len2); break;
      case R_eq:
        retval =  _p_str_eq (s1, len1, s2, len2); break;
      case R_lt:
        retval =  _p_str_lt (s1, len1, s2, len2); break;
      case R_gt:
        retval =  _p_str_lt (s2, len2, s1, len1); break;
      case R_ne:
        retval = !_p_str_eq (s1, len1, s2, len2); break;
      case R_le:
        retval = !_p_str_lt (s2, len2, s1, len1); break;
      case R_ge:
        retval = !_p_str_lt (s1, len1, s2, len2); break;
      case R_INDEX:
        retval =  _p_index (s1, len1, s2, len2);  break;
      default:
        _p_internal_error (909); /* unknown string function called */
    };
    va_end (p);
    RESTORE_RETURN_ADDRESS;
    return retval;
}

/* Read the various integer types */
#define read_check NoRangeCheck
/* Input range checking is not supported in _p_read() and _p_readstr() that
   the following macro belongs to, but it is supported in _p_readi_check(). */
#define READ_INT(sign, type, type2) \
  if (read_check != NoRangeCheck) \
    { \
      sign type *i = va_arg (p, sign type *); \
      sign long long min = va_arg (p, sign type2); \
      sign long long max = va_arg (p, sign type2); \
      *i = (sign long long) _p_readi_check (File, read_check, min, max); \
    } \
  else \
    *(va_arg (p, sign type *)) = (sign long long) _p_readi (File); \
  break;

/* common to _p_read() and _p_readstr() */
#define READ_VARIOUS_TYPES \
  case P_S_BYTE:     READ_INT (signed, char, int) \
  case P_S_SHORT:    READ_INT (signed, short, int) \
  case P_S_INT:      READ_INT (signed, int, int) \
  case P_S_LONG:     READ_INT (signed, long, long long) \
  case P_S_LONGLONG: READ_INT (signed, long long, long long) \
  case P_U_BYTE:     READ_INT (unsigned, char, int) \
  case P_U_SHORT:    READ_INT (unsigned, short, int) \
  case P_U_INT:      READ_INT (unsigned, int, int) \
  case P_U_LONG:     READ_INT (unsigned, long, long long) \
  case P_U_LONGLONG: READ_INT (unsigned, long long, long long) \
  case P_SHORT_REAL: \
    *(va_arg (p, float *)) = _p_read_shortreal (File); \
    break; \
  case P_REAL: \
    *(va_arg (p, double *)) = _p_read_real (File); \
    break; \
  case P_LONG_REAL: \
    *(va_arg (p, long double *)) = _p_read_longreal (File); \
    break; \
  case P_CHAR: \
    *(va_arg (p, char *)) = _p_read_char (File); \
    break; \
  case P_BOOL: \
    *(va_arg (p, Boolean *)) = _p_read_boolean (File); \
    break; \
  case P_ENUM: \
    { \
      char **IDs = va_arg (p, char **); \
      int IDCount = va_arg (p, int); \
      *(va_arg (p, int *)) = _p_read_enum (File, IDs, IDCount); \
      break; \
    } \
  case P_STRING: \
    { \
      char *str = va_arg (p, char *); /* pointer to string */ \
      int *s_curlen = va_arg (p, int *); /* pointer to current string length */ \
      int s_maxlen  = va_arg (p, int); /* maximum length */ \
      if (!s_curlen) _p_internal_error (908); /* Incorrect reading of string */ \
      *s_curlen = _p_read_string (File, str, s_maxlen); \
      break; \
    } \
  case P_SHORT_STRING: \
    { \
      char *str = va_arg (p, char *); /* pointer to string */ \
      char *s_curlen = va_arg (p, char *); /* pointer to current string length */ \
      int s_maxlen  = va_arg (p, int); /* maximum length */ \
      if (!s_curlen) _p_internal_error (908); /* Incorrect reading of string */ \
      *s_curlen = _p_read_string (File, str, s_maxlen); \
      break; \
    } \
  case P_FIXED_STRING: \
    { \
      char *str = va_arg (p, char *); /* pointer to string */ \
      int s_maxlen = va_arg (p, int); /* maximum length */ \
      int length = _p_read_string (File, str, s_maxlen); \
      while (length < s_maxlen) str [length++] = ' '; /* fill with spaces */ \
      break; \
    } \
  case P_CSTRING: \
    { \
      char *str = va_arg (p, char *); /* pointer to string */ \
      int s_maxlen = va_arg (p, int); /* maximum length */ \
      int curlen = _p_read_string (File, str, s_maxlen - 1); /* reserve space for terminator */ \
      str [curlen] = 0; /* Add #0 terminator for CStrings */ \
      break; \
    }

/* read various things from TEXT files
   _p_read does not return immediately when _p_inoutres is set, but loops
   through all the arguments and sets them to "zero" values */
void
_p_read (File, count, va_alist)
FDR File;
int count;
va_dcl
{
  va_list p;
  _p_read_init (File, 0);
  va_start (p);
  while (count--)
    {
      int Flags = va_arg (p, int); /* Type we are reading */
      /* The type code is only
         needed for the following switch and can be removed when the
         switch is "built in".

         The subroutines only need the modifier flags. They should
         be constant during one Read{,ln,Str} call, and can therefore
         be initialized once in the _p_read_init() call above, eliminating
         the following assignment. */
      File->Flags = Flags;

      switch (Flags & INPUT_TYPE_MASK)
      {
        READ_VARIOUS_TYPES
        case P_LINE:
          if (count != 0)
            _p_internal_error (900); /* Compiler calls `Readln' incorrectly */
          _p_readln (File);
          break;
        default:
          _p_internal_error (902); /* unknown code in `Read' */
      }
    }
  va_end (p);
}

/* read from a string
   _p_readstr does not return immediately when _p_inoutres is set, but loops
   through all the arguments and sets them to "zero" values */
void
_p_readstr (string, maxchars, count, va_alist)
char *string;
int maxchars, count;
va_dcl
{
  va_list p;
  struct Fdr TempFile; /* This is no real file, be careful what you do with it. Don't call initfdr(). ;*/
  FDR File = &TempFile; /* needed by READ_VARIOUS_TYPES */

  va_start (p);

  _p_readstr_init (File, string, ((maxchars == -1) ? _p_strlen (string) /* CString */
                                                   : maxchars           /* Pascal string */ ),
                   READ_WRITE_STRING_MASK);

  while (count--)
    {
      int Flags = va_arg (p, int);
      /* cf. the comment in _p_read(), but note READ_WRITE_STRING_MASK */
      File->Flags = READ_WRITE_STRING_MASK | Flags;

      switch (Flags & INPUT_TYPE_MASK)
      {
        READ_VARIOUS_TYPES
        default:
          _p_internal_error (903); /* unknown code in `ReadStr' */
      }
    }
  va_end (p);
}

#define VAL_ROUTINE(FUNCTION) \
{ \
  struct Fdr TempFile; /* This is no real file, be careful what you do with it. Don't call initfdr(). ;*/ \
  _p_val_init (&TempFile, string, ((maxchars == -1) ? _p_strlen (string) : maxchars), \
               READ_WRITE_STRING_MASK | VAL_MASK | flags); \
  *var = FUNCTION; \
  return _p_get_val_return_value (&TempFile); \
}

#define VAL_REAL(func, type, read_func) \
int func (char *, int, int, type *); \
int func (string, maxchars, flags, var) \
char *string; int maxchars, flags; type *var; \
VAL_ROUTINE (read_func (&TempFile))

VAL_REAL (_p_val_shortreal, float,       _p_read_shortreal)
VAL_REAL (_p_val_real,      double,      _p_read_real)
VAL_REAL (_p_val_longreal,  long double, _p_read_longreal)

/* read from a string into one integer argument */
#define VAL_INT_NOCHECK(func, sign, type) \
int func (char *, int, int, sign type *); \
int func (string, maxchars, flags, var) \
char *string; int maxchars, flags; sign type *var; \
VAL_ROUTINE (_p_readi (&TempFile))

VAL_INT_NOCHECK (_p_val_byteint_nocheck,   signed,   char)
VAL_INT_NOCHECK (_p_val_shortint_nocheck,  signed,   short)
VAL_INT_NOCHECK (_p_val_integer_nocheck,   signed,   int)
VAL_INT_NOCHECK (_p_val_medint_nocheck,    signed,   long)
VAL_INT_NOCHECK (_p_val_longint_nocheck,   signed,   long long)
VAL_INT_NOCHECK (_p_val_bytecard_nocheck,  unsigned, char)
VAL_INT_NOCHECK (_p_val_shortcard_nocheck, unsigned, short)
VAL_INT_NOCHECK (_p_val_cardinal_nocheck,  unsigned, int)
VAL_INT_NOCHECK (_p_val_medcard_nocheck,   unsigned, long)
VAL_INT_NOCHECK (_p_val_longcard_nocheck,  unsigned, long long)

#if 0 /* so they don't waste space in libgpc.a -- not implemented in the compiler yet, anyway */
#define VAL_INT_CHECK(func, check, sign, type) \
int func (char *, int, int, sign type *, sign type, sign type); \
int func (char *string, int maxchars, int flags, sign type *var, sign type min, sign type max) \
VAL_ROUTINE (_p_readi_check (&TempFile, check, (sign long long) min, (sign long long) max))

VAL_INT_CHECK (_p_val_byteint_check,   SignedRangeCheck,   signed,   char)
VAL_INT_CHECK (_p_val_shortint_check,  SignedRangeCheck,   signed,   short)
VAL_INT_CHECK (_p_val_integer_check,   SignedRangeCheck,   signed,   int)
VAL_INT_CHECK (_p_val_medint_check,    SignedRangeCheck,   signed,   long)
VAL_INT_CHECK (_p_val_longint_check,   SignedRangeCheck,   signed,   long long)
VAL_INT_CHECK (_p_val_bytecard_check,  UnsignedRangeCheck, unsigned, char)
VAL_INT_CHECK (_p_val_shortcard_check, UnsignedRangeCheck, unsigned, short)
VAL_INT_CHECK (_p_val_cardinal_check,  UnsignedRangeCheck, unsigned, int)
VAL_INT_CHECK (_p_val_medcard_check,   UnsignedRangeCheck, unsigned, long)
VAL_INT_CHECK (_p_val_longcard_check,  UnsignedRangeCheck, unsigned, long long)
#endif

/* common to _p_write() and _p_writestr() */
#define WRITE_VARIOUS_TYPES \
  case P_S_INT: \
    { \
      signed int num = va_arg (p, signed int); \
      _p_write_integer (File, num, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    } \
  case P_S_LONGLONG: \
    { \
      signed long long int num = va_arg (p, signed long long int); \
      _p_write_longint (File, num, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    } \
  case P_U_INT: \
    { \
      unsigned int num = va_arg (p, unsigned int); \
      _p_write_cardinal (File, num, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    } \
  case P_U_LONGLONG: \
    { \
      unsigned long long int num = va_arg (p, unsigned long long int); \
      _p_write_longcard (File, num, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    } \
  case P_LONG_REAL: \
    { \
      long double num = va_arg (p, long double); \
      int width = (File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer; \
      int prec = (File->Flags & FIX2_REAL_MASK) ? va_arg (p, int) : _p_low_integer; \
      _p_write_real (File, num, width, prec); \
      break; \
    } \
  case P_CHAR: \
    { \
      char ch = (char) va_arg (p, int); \
      _p_write_char (File, ch, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    } \
  case P_BOOL: \
    { \
      int b = va_arg (p, int); \
      _p_write_boolean (File, b, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    } \
  case P_ENUM: \
    { \
      char **IDs = va_arg (p, char **); \
      int IDCount = va_arg (p, int); \
      int v = va_arg (p, int); \
      _p_write_enum (File, IDs, IDCount, v, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    } \
  case P_ANY_STRING: \
    { \
      char *str = va_arg (p, char *); \
      int length = va_arg (p, int); \
      _p_write_string (File, str, length, ((File->Flags & FIX_WIDTH_MASK) ? va_arg (p, int) : _p_low_integer)); \
      break; \
    }

void
_p_write (File, count, va_alist)
FDR File;
int count;
va_dcl
{
  va_list p;
  if (_p_inoutres) return;
  _p_write_init (File, 0);
  if (_p_inoutres) return;
  va_start (p);
  while (count--)
    {
      int Flags = va_arg (p, int); /* Type we are writing */
      /* The type code and FIX_WIDTH_MASK and FIX2_REAL_MASK are only
         needed for the following switch and can be removed when the
         switch is "built in".

         The subroutines only need the other modifier flags. They should
         be constant during one Write{,ln,Str} call, and can therefore
         be initialized once in the _p_write_init() call above, eliminating
         the following assignment. */
      File->Flags = Flags;

      switch (Flags & OUTPUT_TYPE_MASK)
      {
        WRITE_VARIOUS_TYPES
        case P_LINE:
          if (count != 0)
            _p_internal_error (901); /* Compiler calls `Writeln' incorrectly */
          _p_writeln (File);
          break;
        default:
          _p_internal_error (904); /* unknown code in `Write' */
      }
    }
  va_end (p);
  _p_write_flush (File);
}

/* string_type must be one of P_STRING, P_SHORT_STRING, P_FIXED_STRING,
   P_CSTRING, possibly ORed with TRUNCATE_STRING_MASK */
void
_p_writestr (string_type, s1, va_alist)
int string_type;
char *s1;
va_dcl
{
  va_list p;
  int count, Capacity, Flags1, Length;
  int *long_curlen = 0;
  char *short_curlen = 0;
  struct Fdr TempFile; /* This is no real file, be careful what you do with it. Don't call initfdr(). ;*/
  FDR File = &TempFile; /* needed by WRITE_VARIOUS_TYPES */

  /* Don't check _p_inoutres here because this is called
     (indirectly) in the writing of error messages! */

  va_start (p);

  /* Length pointer. Only needed at the end of the routine. */
  switch (string_type & OUTPUT_TYPE_MASK)
  {
    case P_STRING:
      long_curlen = va_arg (p, int *);
      break;
    case P_SHORT_STRING:
      short_curlen = va_arg (p, char *);
      break;
    case P_FIXED_STRING:
    case P_CSTRING:
      break;
    default:
      _p_internal_error (906); /* unknown string code in `WriteStr' */
  }

  Capacity = va_arg (p, int);
  /* If it's a CString, reserve space for the #0 terminator */
  if ((string_type & OUTPUT_TYPE_MASK) == P_CSTRING)
    if (--Capacity < 0)
      _p_internal_error (907); /* string length cannot be negative */

  Flags1 = READ_WRITE_STRING_MASK | (string_type & TRUNCATE_STRING_MASK);
  _p_writestr_init (File, s1, Capacity, Flags1);

  count = va_arg (p, int);
  while (count--)
    {
      int Flags = va_arg (p, int);
      /* cf. the comment in _p_write(), but note Flags1 */
      File->Flags = Flags1 | Flags;

      switch (Flags & OUTPUT_TYPE_MASK)
      {
        WRITE_VARIOUS_TYPES
        default:
          _p_internal_error (905); /* unknown code in `WriteStr' */
      }
    }
  va_end (p);

  Length = _p_writestr_getlength (File);

  /* Set the current string length */
  /* The following is roughly the code for an "AnyString SetLength" */
  switch (string_type & OUTPUT_TYPE_MASK)
  {
    case P_STRING:
      *long_curlen = Length;
      break;
    case P_SHORT_STRING:
      *short_curlen = Length;
      break;
    case P_FIXED_STRING:
      while (Length < Capacity)
        s1 [Length++] = ' ';
      break;
    case P_CSTRING:
      s1 [Length] = 0;
      break;
  }
}
