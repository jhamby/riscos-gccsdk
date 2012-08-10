/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2002-2012 GCCSDK Developers
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
 * local.h
 */

#ifndef local_header_included
#define local_header_included

#include <stdbool.h>
#include <stddef.h>

#include "lex.h"
#include "phase.h"

typedef enum
{
  eThisLevelOnly,	/* 't' */
  eAllLevels,		/* 'a' */
  eThisLevelAndHigher	/* '' */
} LocalLabel_eLevel;

typedef enum
{
  eBackward,		/* 'b' */
  eBackwardThenForward,	/* '' */
  eForward		/* 'f' */
} LocalLabel_eDir;

typedef struct Local_Label_t
{
  struct Local_Label_t *nextP; /* Must be first.  */

  unsigned num; /**< Label number.  */
  unsigned instance; /**< Instance of label number.  */
} Local_Label_t;

void Local_PrepareForPhase (Phase_e phase);

Local_Label_t *Local_DefineLabel (unsigned num);
void Local_CreateSymbolForOutstandingFwdLabelRef (char *buf, size_t bufSize,
						  LocalLabel_eLevel level,
						  LocalLabel_eDir dir,
						  unsigned label);
Local_Label_t *Local_GetLabel (unsigned depth, unsigned num);

void Local_CreateSymbol (Local_Label_t *lblP, unsigned macroDepth, bool next, char *buf, size_t bufSize);

void Local_FinishMacro (bool noCheck);

bool c_rout (const Lex *label);

bool Local_IsLocalLabel (const char *);
const char *Local_GetCurROUTId (const char **fileNamePP, unsigned *lineNumP);

#ifdef DEBUG
void Local_DumpAll (void);
#endif

#endif
