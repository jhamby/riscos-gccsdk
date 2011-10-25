/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2011 GCCSDK Developers
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
 * lex.h
 */

#ifndef lex_header_included
#define lex_header_included

#include <stdbool.h>
#include <stddef.h>

#include "global.h"

/* Ordered according to decreasing precedence.  */
typedef enum
{
  /* Unary operators.  */
  Op_fload = 0, Op_fexec, Op_fsize, Op_fattr,	/* Pri 7 */
  Op_lnot, Op_not, Op_neg, Op_none,		/* Pri 7 */
  Op_base, Op_index, Op_len, Op_str, Op_chr,	/* Pri 7 */
  Op_size,					/* Pri 7 */

  /* Binary operators.  */
  Op_mul, Op_div, Op_mod,			/* Pri 6 */
  Op_left, Op_right, Op_concat,			/* Pri 5 */
  Op_asr, Op_sr, Op_sl, Op_ror, Op_rol,		/* Pri 4 */
  Op_add, Op_sub, Op_and, Op_or, Op_xor,	/* Pri 3 */
  Op_le, Op_ge, Op_lt, Op_gt, Op_eq, Op_ne,	/* Pri 2 */
  Op_land, Op_lor, Op_leor			/* Pri 1 */
} Operator;

#define kPrioOp_Unary		7
#define kPrioOp_Multiplicative	6
#define kPrioOp_String		5
#define kPrioOp_Shift		4
#define kPrioOp_AddAndLogical	3
#define kPrioOp_Relational	2
#define kPrioOp_Boolean		1

#define kPrioOp_Max		7
#define kPrioOp_Min		1

#ifdef DEBUG
const char *Lex_OperatorAsStr (Operator op);
#endif

static inline bool
IsUnop (Operator op)
{
  return op <= Op_size;
}

typedef enum
{
  LexId,			/* start with character */
  LexLocalLabel,		/* start with digit */
  LexString,			/* "jsgd" */
  LexInt,			/* start with digit */
  LexFloat,			/* start with digit contains dot */
  LexOperator,			/* + - * % / >> >>> << */
				/* ~ & | ^ ! && || */
				/* == != <= >= */
  LexPosition,			/* . representing current position */
  LexStorage,			/* @ representing current storage counter */
  LexDelim,			/* () */
  LexBool,			/* {TRUE} or {FALSE} */
  LexNone
} LexTag;

typedef struct
{
  LexTag tag; /* FIXME: change into Tag */
  union
    {
      struct			/* LexId */
        {
          const char *str;	/* *NOT* NUL terminated.  */
          size_t len;
          unsigned int hash;
        } Id;
      struct			/* LocalLabel */
	{
          const char *str;	/* *NOT* NUL terminated.  */
          size_t len;
	} LocalLabel;
      struct			/* LexString */
        {
          const char *str;	/* *NOT* NUL terminated.  */
          size_t len;
        } String;
      struct			/* LexInt */
        {
          int value;
        } Int;
      struct			/* LexFloat */
        {
          ARMFloat value;
        } Float;
      struct			/* LexOperator */
        {
          Operator op;
          int pri;
        } Operator;
      struct			/* LexDelim */
        {
          char delim;
        } Delim;
      struct			/* LexBool */
        {
          bool value;
        } Bool;
    } Data;
} Lex;

Lex Lex_GetDefiningLabel (void);
bool Lex_SkipDefiningLabel (void);
Lex Lex_DefineLocalLabel (const Lex *lexP);
Lex lexGetId (void);
Lex lexGetIdNoError (void);
Lex lexGetPrim (void);
Lex lexGetBinop (void);
int lexNextPri (void);

Lex lexTempLabel (const char *str, size_t len);

#ifdef DEBUG
void lexPrint (const Lex *lex);
#endif

#endif
