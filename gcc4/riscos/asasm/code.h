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
 * code.h
 */

#ifndef code_header_included
#define code_header_included

#include <stdbool.h>

#include "global.h"
#include "lex.h"
#include "reloc.h"
#include "symbol.h"
#include "value.h"

typedef enum
{
  CodeOperator,
  CodeValue,
  CodeSymbol
} CodeTag;

typedef union CODE
{
  CodeTag Tag;
  struct
    {
      CodeTag Tag;
      Operator op;
    } CodeOperator;
  struct
    {
      CodeTag Tag;
      Value value;
    } CodeValue;
  struct
    {
      CodeTag Tag;
      Symbol *symbol;
    } CodeSymbol;
} Code;

#define CODE_SIZECODE  (1024)
#define CODE_SIZESTACK (1024)
#define CODE_SIZELATE  (1024)

void codeInit (void);

void codeOperator (Operator op);
void codeSymbol (Symbol *symbol);
void codeInt (int value);
void codePosition (Symbol *area);
void codeStorage (void);
void codeString (int len, const char *str);
void codeFloat (ARMFloat value);
void codeBool (bool value);

Value codeEvalLow (ValueTag legal, int size, Code *program);
Value codeEval (ValueTag legal);

LateInfo *codeNewLateInfo (Symbol *symbol);
Code *codeCopy (int len, const Code *code);
bool codeEqual (int len, const Code *a, const Code *b);

#endif
