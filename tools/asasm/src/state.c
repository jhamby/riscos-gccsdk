/*
 * AS an assembler for ARM
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * state.c
 */

#include "config.h"

#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "error.h"
#include "phase.h"
#include "state.h"
#include "targetcpu.h"

static SyntaxInstrState_e oDefaultSyntaxInstrType = eState_Code32;

static Syntax_e oCurrentSyntax;
static InstrType_e oCurrentInstrType;


void
State_Set (SyntaxInstrState_e syntaxInstrType)
{
  oDefaultSyntaxInstrType = syntaxInstrType;
}


static void
State_Init (SyntaxInstrState_e syntaxInstrType)
{
  switch (syntaxInstrType)
    {
      case eState_Code32:
	oCurrentInstrType = eInstrType_ARM;
	oCurrentSyntax = eSyntax_Both;
	Target_CheckCPUFeatureDetail (kArchExt_v1, "ARM instruction set");
	break;

      case eState_Code16:
	oCurrentInstrType = eInstrType_Thumb;
	oCurrentSyntax = eSyntax_PreUALOnly;
	/* Assume v6T2 -> v4T Thumb support as well.  */
	Target_CheckCPUFeatureDetail (kCPUExt_v4T, "Thumb instruction set");
	break;

      case eState_Thumb:
	oCurrentInstrType = eInstrType_Thumb;
	oCurrentSyntax = eSyntax_UALOnly;
	/* Assume v6T2 -> v4T Thumb support as well.  */
	Target_CheckCPUFeatureDetail (kCPUExt_v4T, "Thumb instruction set");
	break;

      case eState_ThumbEE:
	oCurrentInstrType = eInstrType_ThumbEE;
	oCurrentSyntax = eSyntax_UALOnly;
	Target_CheckCPUFeatureDetail (kCPUExt_ThumbEE, "ThumbEE instruction set");
	break;

    }
}


void
State_PrepareForPhase (Phase_e phase)
{
  /* At the start of our two passes, reinitialise to the default or
     command line specified syntax and UAL state.  */
  switch (phase)
    {
      case ePassOne:
      case ePassTwo:
	State_Init (oDefaultSyntaxInstrType);
	break;

      case eStartUp:
      case eOutput:
      case eCleanUp:
	break;
    }
}

Syntax_e
State_GetSyntax (void)
{
  return oCurrentSyntax;
}

InstrType_e
State_GetInstrType (void)
{
  return oCurrentInstrType;
}

/**
 * Implements CODE16.
 * What follows is in pre-UAL Thumb notation.
 * Does *not* support the 32-bit Thumb instructions.
 * If necessary, this directives also insert up to one byte of padding to
 * align to the next halfword boundary for Thumb or ThumbEE.
 */
bool
c_code16 (void)
{
  State_Init (eState_Code16);
  Area_AlignArea (areaCurrentSymbol, 2, NULL);
  return false;
}

/**
 * Implements THUMB.
 * What follows in in UAL Thumb notation.
 * If necessary, this directives also insert up to one byte of padding to
 * align to the next halfword boundary for Thumb or ThumbEE.
 */
bool
c_thumb (void)
{
  State_Init (eState_Thumb);
  Area_AlignArea (areaCurrentSymbol, 2, NULL);
  return false;
}

/**
 * Implements THUMBX.
 * What follows is in UAL Thumb notation.
 * If necessary, this directives also insert up to one byte of padding to
 * align to the next halfword boundary for Thumb or ThumbEE.
 */
bool
c_thumbx (void)
{
  State_Init (eState_ThumbEE);
  Area_AlignArea (areaCurrentSymbol, 2, NULL);
  return false;
}

/**
 * Implements ARM / CODE32.
 * What follows can be in UAL or pre-UAL ARM notation.
 * If necessary, this directive also insert up to three bytes of padding to
 * align to the next word boundary for ARM.
 */
bool
c_code32 (void)
{
  State_Init (eState_Code32);
  Area_AlignArea (areaCurrentSymbol, 4, NULL);
  return false;
}
