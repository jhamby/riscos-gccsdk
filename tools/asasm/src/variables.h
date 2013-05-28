/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2013 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef variables_header_included
#define variables_header_included

#include "lex.h"
#include "phase.h"
#include "symbol.h"

/**
 * Created for each local variable encountered in a macro as we need to restore
 * it.
 */
typedef struct VarPos
{
  const struct VarPos *next;
  Symbol *symbolP; /**< Non-NULL when macro caller has this variable already defined.  */
  Symbol symbol; /**< When symbolP is non-NULL, the previous symbol object.  */
  char name[]; /**< NUL terminated symbol name.  */
} VarPos;

bool c_gbl (void);
bool c_lcl (void);
bool c_set (const Lex *label);

void Var_PrepareForPhase (Phase_e phase);
void Var_RestoreLocals (const VarPos *);	/* called on macro exit */

#endif
