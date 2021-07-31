/*
 * AsAsm an assembler for ARM
 * Copyright (c) 2010-2013 GCCSDK Developers
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

#ifndef state_header_included
#define state_header_included

#include <stdbool.h>

#include "phase.h"

typedef enum
{
  eSyntax_PreUALOnly,
  eSyntax_UALOnly,
  eSyntax_Both
} Syntax_e;

typedef enum
{
  eInstrType_ARM,
  eInstrType_Thumb,
  eInstrType_ThumbEE
} InstrType_e;

typedef enum
{
  eState_Code32,	/* Pre-UAL and UAL, ARM.  */
  eState_Code16,	/* Pre-UAL only, Thumb.  */
  eState_Thumb,		/* UAL only, Thumb.  */
  eState_ThumbEE	/* UAL only, ThumbEE.  */
} SyntaxInstrState_e;

void State_Set (SyntaxInstrState_e syntaxInstrType);

void State_PrepareForPhase (Phase_e phase);

Syntax_e State_GetSyntax (void);
InstrType_e State_GetInstrType (void);

bool c_code16 (void);
bool c_code32 (void);
bool c_thumb (void);
bool c_thumbx (void);

#endif
