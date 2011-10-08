/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2011 GCCSDK Developers
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
 * asm.h
 */

#ifndef asm_header_included
#define asm_header_included

#include "lex.h"
#include "symbol.h"

typedef enum
{
  eStartup,
  ePassOne,
  ePassTwo,
  eOutput
} ASM_Phase_e;
extern ASM_Phase_e gASM_Phase;

void ASM_Assemble (const char *asmFile);
Symbol *ASM_DefineLabel (const Lex *label, int offset);

#endif
