/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2010 GCCSDK Developers
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * value.h
 */

#ifndef value_header_included
#define value_header_included

#include <stdbool.h>
#include <stddef.h>

#include "global.h"

struct Symbol;		/* Cannot include symbol.h as it needs value.h */
struct Code;

typedef enum
{
  ValueIllegal   =   0,
  ValueInt       =   1,
  ValueFloat     =   2,
  ValueString    =   4,
  ValueBool      =   8,
  ValueCode      =  16,
  ValueAddr      =  32,
  ValueSymbol    =  64,

  ValueAll       = 127		/* cheat */
} ValueTag;

typedef struct
{
  ValueTag Tag;
  union
    {
      struct			/* ValueInt */
        {
          int i;		/* Must start identical with ValueAddr's i & ValueString's len.  */
        } Int;
      struct			/* ValueFloat */
        {
          ARMFloat f;
        } Float;
      struct			/* ValueString */
        {
	  size_t len;		/**< Size string.  Must start identical with ValueInt's i & ValueAddr's i.  */
	  const char *s;	/**< Malloced memory block and string contents is *NOT* NUL terminated.  */
        } String;
      struct			/* ValueBool */
        {
          bool b;
        } Bool;
      struct			/* ValueCode */
        {
          size_t len;
          struct Code *c;
        } Code;
      struct			/* ValueAddr, represents address in the form of "[<r>, #<i>]" */
        {
	  int i;		/* Must start identical with ValueInt's i & ValueStrings's len.  */
          int r;		/* When = 0 - 15 (inc), it is register based. -1 otherwise.  */
        } Addr;
      struct			/* ValueSymbol */
	{
	  int factor;		/* Number of times the symbol needs to be taken into account (can be negative, zero, positive).  */
	  struct Symbol *symbol;
	  int offset;
	} Symbol;
    } Data;
} Value;

static inline Value
Value_Int (int i)
{
  const Value value =
    {
      .Tag = ValueInt,
      .Data.Int.i = i
    };
  return value;
}

static inline Value
Value_Float (ARMFloat f)
{
  const Value value =
    {
      .Tag = ValueFloat,
      .Data.Float.f = f
    };
  return value;
}

Value Value_Code (size_t len, const struct Code *code);

static inline Value
Value_Addr (int reg, int i)
{
  const Value value =
    {
      .Tag = ValueAddr,
      .Data.Addr = { .i = i, .r = reg }
    };
  return value;
}

static inline Value
Value_Symbol (struct Symbol *symbol, int factor, int offset)
{
  const Value value =
    {
      .Tag = ValueSymbol,
      .Data.Symbol = { .factor = factor, .symbol = symbol, .offset = offset }
    };
  return value;
}

void Value_Assign (Value *dst, const Value *src);
void valueFree (Value *value);
bool valueEqual (const Value *a, const Value *b);
const char *valueTagAsString (ValueTag tag);
#ifdef DEBUG
void valuePrint (const Value *v);
#endif
#endif
