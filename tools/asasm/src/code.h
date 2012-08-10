/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2012 GCCSDK Developers
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
  CodeValue
} CodeTag;

typedef struct Code
{
  CodeTag Tag;
  union
    {
      Operator_e op;	/* .Tag = CodeOperator */
      Value value;	/* .Tag = CodeValue */
    } Data;
} Code;

void codeInit (void);

void codeOperator (Operator_e op);
void codeSymbol (Symbol *symbol, int offset);
void codeInt (int value);
void codePosition (Symbol *area, int offset);
void codeStorage (void);
void codeString (const char *str, size_t len);
void codeFloat (ARMFloat value);
void codeBool (bool value);
void codeAddr (int reg, int offset);
void codeValue (const Value *value, bool expCode);

const Value *codeEvalLow (ValueTag legal, size_t size, const Code *program, const ARMWord *instrOffsetP);
const Value *codeEval (ValueTag legal, const ARMWord *instrOffsetP);

Value Code_TakeSnapShot (void);

void Code_Free (const Code *code, size_t len);
Code *codeCopy (size_t len, const Code *code);
bool codeEqual (size_t len, const Code *a, const Code *b);

#ifdef DEBUG
void codePrint (size_t size, const Code *program);
#endif

#endif
