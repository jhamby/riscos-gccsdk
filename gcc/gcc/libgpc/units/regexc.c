/*
Support routines for regex.pas

Copyright (C) 1998-2000 Free Software Foundation, Inc.

Author: Frank Heckenbach <frank@pascal.gnu.de>

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

#include <stdlib.h>
#include <rxposix.h>

typedef unsigned char Boolean;

typedef struct
{
  regex_t    *RegEx;
  regmatch_t *RegMatch;
  int         From, Length, SubExpressions;
  char       *Error;
} RegExType;

void _p_error_regex (RegExType *r, int errcode);
void _p_error_regex (RegExType *r, int errcode)
{
  if (r -> Error)
    {
      free (r -> Error);
      r -> Error = 0;
    }
  if (errcode)
    {
      size_t size = regerror (errcode, r -> RegEx, 0, 0);
      r -> Error = (char *) malloc (size);
      regerror (errcode, r -> RegEx, r -> Error, size);
    }
}

void _p_new_regex (RegExType *r, const char *Expression, int ExpressionLength,
                   Boolean ExtendedRegEx, Boolean CaseInsensitive, Boolean NewLines);
void _p_new_regex (RegExType *r, const char *Expression, int ExpressionLength,
                   Boolean ExtendedRegEx, Boolean CaseInsensitive, Boolean NewLines)
{
  int result;
  r -> Error = 0;
  r -> From = 1;
  r -> Length = 0;
  r -> SubExpressions = 0;
  r -> RegMatch = 0;
  r -> RegEx = (regex_t *) malloc (sizeof (regex_t));
  result = regncomp (r -> RegEx, Expression, ExpressionLength,
            (ExtendedRegEx    ? REG_EXTENDED : 0) |
            (CaseInsensitive  ? REG_ICASE    : 0) |
            (NewLines         ? REG_NEWLINE  : 0));
  if (result)
    {
      _p_error_regex (r, result);
      free (r -> RegEx);
      r -> RegEx = 0;
    }
  else
    {
      r -> SubExpressions = r -> RegEx -> re_nsub - 1;
      r -> RegMatch = (regmatch_t *) malloc ((r -> SubExpressions + 1) * sizeof (regmatch_t));
    }
}

void _p_dispose_regex (RegExType *r);
void _p_dispose_regex (RegExType *r)
{
  if (r -> RegEx)
    {
      regfree (r -> RegEx);
      free (r -> RegEx);
      r -> RegEx = 0;
    }
  if (r -> Error)
    {
      free (r -> Error);
      r -> Error = 0;
    }
  if (r -> RegMatch)
    {
      free (r -> RegMatch);
      r -> RegMatch = 0;
      r -> SubExpressions = -1;
    }
}

Boolean _p_match_regex_from (RegExType *r, const char *aString, int StrLength,
                             Boolean NotBeginningOfLine, Boolean NotEndOfLine, int From);
Boolean _p_match_regex_from (RegExType *r, const char *aString, int StrLength,
                             Boolean NotBeginningOfLine, Boolean NotEndOfLine, int From)
{
  int i = r -> Length = StrLength - From + 1;
  r -> From = From;
  return !regnexec (r -> RegEx, (i >= 0) ? aString + From - 1 : aString, (i >= 0) ? i : 0, r -> SubExpressions + 1,
                    &r -> RegMatch, (NotBeginningOfLine ? REG_NOTBOL : 0) |
                    (NotEndOfLine ? REG_NOTEOL : 0));
}

void _p_getmatch_regex (RegExType *r, int n, int *MatchPosition, int *MatchLength);
void _p_getmatch_regex (RegExType *r, int n, int *MatchPosition, int *MatchLength)
{
  int MPosition, MLength;
  if (n < 0 || n > r -> SubExpressions)
    MPosition = MLength = 0;
  else
    {
      MPosition = r -> RegMatch [n].rm_so;
      MLength   = r -> RegMatch [n].rm_eo;
      if (MPosition >= 0 && MLength >= 0 && MLength <= r -> Length)
        {
          MLength -= MPosition;
          MPosition += r -> From;
        }
      else
        MPosition = MLength = 0;
    }
  if (MatchPosition) *MatchPosition = MPosition;
  if (MatchLength) *MatchLength = MLength;
}
