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

#include "global.h"

struct SYMBOL;		/* Cannot include symbol.h as it needs value.h */

union CODE;

typedef struct LATEINFO
{
  struct LATEINFO *next;
  int factor;
  struct SYMBOL *symbol;
} LateInfo;

typedef enum
{
  ValueIllegal   = 0,
  ValueInt       = 1,
  ValueFloat     = 2,
  ValueString    = 4,
  ValueBool      = 8,
  ValueCode      = 16,
  ValueLateLabel = 32,
  ValueAddr      = 64,

  ValueConst     = 0,		/* only for variables */
  ValueAll       = 127		/* cheat */
} ValueTag;

typedef union
{
  ValueTag Tag;
  struct			/* ValueInt */
    {
      ValueTag Tag;
      int i;
    } ValueInt;
  struct			/* ValueFloat */
    {
      ValueTag Tag;
      ARMFloat f;
    } ValueFloat;
  struct			/* ValueString */
    {
      ValueTag Tag;
      int len;			/**< Size string.  */
      const char *s;		/**< Malloced memory block and string contents is *NOT* NUL terminated.  */
    } ValueString;
  struct			/* ValueBool */
    {
      ValueTag Tag;
      bool b;
    } ValueBool;
  struct			/* ValueCode */
    {
      ValueTag Tag;
      int len;
      union CODE *c;
    } ValueCode;
  struct			/* ValueLateLabel */
    {
      ValueTag Tag;		/* Must start identical with ValueInt */
      int i;
      struct LATEINFO *late;
    } ValueLate;
  struct			/* ValueAddr */
    {
      ValueTag Tag;
      int i;
      int r;			/* When = 0 - 15 (inc), it is register based. -1 otherwise.  */
    } ValueAddr;
} Value;

Value valueLateToCode (int offset, LateInfo *late);
Value valueCopy (Value value);
void valueFree (Value *value);
bool valueEqual (const Value *a, const Value *b);
const char *valueTagAsString (ValueTag tag);
#ifdef DEBUG
void valuePrint (const Value *v);
#endif
#endif
