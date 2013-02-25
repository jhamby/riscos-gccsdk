/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2002-2013 GCCSDK Developers
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
 * macros.h
 */

#ifndef macros_header_included
#define macros_header_included

#include <stdbool.h>

#include "lex.h"
#include "phase.h"
#include "variables.h"

#define MACRO_ARG_LIMIT (18)
/* Characters MACRO_ARG0 up to MACRO_ARG0 + MACRO_ARG_LIMIT may not appear
 * in assembler source code.
 */
#define MACRO_ARG0  14

typedef struct Macro
{
  struct Macro *nextP;
  const char *name; /**< Name of this macro.  Malloced string.  */
  size_t nameLen; /**< Length of macro name.  */  
  const char *fileName; /**< Filename where this macro has been defined.  */
  const char *buf;
  bool labelArg; /**< When true, macro supports label as one of its arguments.  */
  bool suffixArg; /**< When true, macro name as a suffix as one if its arguments.  */
  unsigned char numArgs; /**< Number of macro arguments (including the label
    one if there is one).  Varies from 0 .. MACRO_ARG_LIMIT - 1.  */
  const char *args[MACRO_ARG_LIMIT]; /**< Macro argument names.  NULL or malloced strings.  */
  const char *defArgs[MACRO_ARG_LIMIT]; /**< Default argument value.  NULL or malloced strings.  */
  unsigned startLineNum; /**< Line number in Macro::fileName where its macro definition starts.  */
} Macro;

typedef struct
{
  const Macro *macro; /**< Pointer to the macro definition which is being executed.  */
  const char *curPtr; /**< Current pointer inside macro buffer Macro::buf.  */
  const char *args[MACRO_ARG_LIMIT]; /**< Current argument values during macro execution.  NULL or malloced strings.  */
  const VarPos *varListP; /**< Linked list of local variables defined in this macro.  */
  unsigned optDirective; /**< Value {OPT} just before macro invocation.
    {OPT} gets restored after macro invocation.  */
} PObject_Macro;

void Macro_PrepareForPhase (Phase_e phase);

void FS_PopMacroPObject (bool noCheck);

bool Macro_Call (const char *macroName, size_t macroNameLen, const Lex *lbl);

bool c_macro (void);
bool c_mexit (void);
bool c_mend (void);

#endif
