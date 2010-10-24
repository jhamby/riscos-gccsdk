/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2010 GCCSDK Developers
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

typedef enum
{
  Op_fload = 0, Op_fexec, Op_fsize, Op_fattr,	/* unop */
  Op_lnot, Op_not, Op_neg, Op_none,	/* unop */
  Op_base, Op_index, Op_len, Op_str, Op_chr,	/* unop */
  Op_left, Op_right,		/* 10 (9) */
  Op_mul, Op_div, Op_mod,	/* 10 (9) */
  Op_add, Op_sub, Op_concat,	/*  9 (8) */
  Op_and,			/*  8 (7) */
  Op_or,			/*  7 (6) */
  Op_xor,			/*  6 (5) */
  Op_asr, Op_sr, Op_sl, Op_ror, Op_rol,	/*  5 (10 if objasm) */
  Op_le, Op_ge, Op_lt, Op_gt,	/*  4 */
  Op_eq, Op_ne,			/*  3 */
  Op_land,			/*  2 */
  Op_lor			/*  1 */
} Operator;

#ifdef DEBUG
const char *OperatorAsStr (Operator op);
#endif

#define isUnop(op) \
  ((op) == Op_fload || (op) == Op_fexec || (op) == Op_fsize || \
   (op) == Op_fattr || (op) == Op_lnot  || (op) == Op_not   || \
   (op) == Op_neg   || (op) == Op_base  || (op) == Op_index || \
   (op) == Op_len   || (op) == Op_str   || (op) == Op_chr)
bool (isUnop) (Operator);

extern const char Pri[2][10];
#define PRI(n) Pri[1 /* FIXME: entry 0 is no longer used.  */][n-1]

typedef enum
{
  LexId,			/* start with character */
  LexString,			/* "jsgd" */
  LexInt,			/* start with digit */
  LexFloat,			/* start with digit contains dot */
  LexOperator,			/* + - * % / >> >>> << */
				/* ~ & | ^ ! && || */
				/* == != <= >= */
  LexPosition,			/* . representing current position */
  LexStorage,			/* @ representing current storage counter */
  LexDelim,			/* ()[]{}, */
  Lex00Label,			/* local (numeric) label */
  LexBool,			/* {TRUE} or {FALSE} */
  LexNone
} LexTag;

typedef struct
{
  LexTag tag;
  union
    {
      struct			/* LexId */
        {
          const char *str;	/* *NOT* NUL terminated.  */
          size_t len;
          unsigned int hash;
        } Id;
      struct			/* LexString */
        {
          const char *str;
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
      struct			/* Lex00Label */
        {
          int value;
        } Label;
      struct			/* LexBool */
        {
          bool value;
        } Bool;
    } Data;
} Lex;

Lex lexGetLabel (void);
Lex lexGetLocal (void);
Lex lexGetId (void);
Lex lexGetIdNoError (void);
Lex lexGetIdMunge (int);
Lex lexGetPrim (void);
Lex lexGetBinop (void);
int lexNextPri (void);

Lex lexTempLabel (const char *str, size_t len);

unsigned int lexHashStr (const char *s, size_t maxn);

#ifdef DEBUG
void lexPrint (const Lex *lex);
#endif

#endif
