/*
 * AS an assembler for ARM
 * Copyright (c) 2010-2012 GCCSDK Developers
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

#include "error.h"
#include "phase.h"
#include "state.h"

static Syntax_e oDefaultSyntax = eSyntax_PreUALOnly;
static InstrType_e oDefaultInstrType = eInstrType_ARM;

static Syntax_e oCurrentSyntax;
static InstrType_e oCurrentInstrType;

void
State_SetCmdLineSyntax (Syntax_e syntax)
{
  assert (gPhase == eStartup);
  oDefaultSyntax = syntax;
}

void
State_SetCmdLineInstrType (InstrType_e instrType)
{
  assert (gPhase == eStartup);
  oDefaultInstrType = instrType;
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
	assert ((oDefaultInstrType != eInstrType_ThumbEE || oDefaultSyntax == eSyntax_UALOnly) && "ThumbEE & pre-UAL is impossible");
	oCurrentSyntax = oDefaultSyntax;
	oCurrentInstrType = oDefaultInstrType;
	break;

      case eStartup:
      case eOutput:
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
 * What follows is in pre-UAL notation.
 */
bool
c_code16 (void)
{
  oCurrentInstrType = eInstrType_Thumb;
  oCurrentSyntax = eSyntax_PreUALOnly;
  return false;
}

/**
 * Implements THUMB.
 * What follows in in UAL notation.
 */
bool
c_thumb (void)
{
  oCurrentInstrType = eInstrType_Thumb;
  oCurrentSyntax = eSyntax_UALOnly;
  return false;
}

/**
 * Implements THUMBX.
 * What follows is in UAL notation.
 */
bool
c_thumbx (void)
{
  oCurrentInstrType = eInstrType_ThumbEE;
  oCurrentSyntax = eSyntax_UALOnly;
  return false;
}

/**
 * Implements ARM / CODE32.
 * What follows can be in UAL or pre-UAL notation.
 */
bool
c_code32 (void)
{
  oCurrentInstrType = eInstrType_ARM;
  oCurrentSyntax = eSyntax_Both;
  return false;
}
