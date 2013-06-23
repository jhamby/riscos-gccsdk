/*
 * AsAsm an assembler for ARM
 * Copyright (c) 2012-2013 GCCSDK Developers
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

#include "config.h"

#include <stdbool.h>
#include <string.h>

#include "area.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "it.h"
#include "option.h"
#include "put.h"
#include "state.h"
#include "symbol.h"
#include "targetcpu.h"

void
IT_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case ePassTwo:
	{
	  for (Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area->next)
	    {
	      IT_State_t *itStateP = &ap->area->it;
	      if (itStateP->curIdx != itStateP->maxIdx)
		{
		  /* We have a pending IT block.  */
		  Error (ErrorError, "There is a pending IT block at end of area %s", ap->str);
		  Error_Line (itStateP->fileName, itStateP->lineNum, ErrorError, "note: Pending IT block started here");
		}
	      /* Reset any implicit IT block.  */
	      IT_InitializeState (&ap->area->it);
	    }
	  break;
	}

      case eStartUp:
      case ePassOne:
      case eOutput:
      case eCleanUp:
	break;
    }
}


/**
 * Emits IT instruction in Thumb code.
 */
static void
IT_EmitIT (const IT_State_t *state, bool isThumb)
{
  /* Only in Thumb state we're outputing an instruction.  */
  if (!isThumb)
    return;

  /* IT Thumb instruction is only available in ARMv6T2 and ARMv7.  In ARM
     mode it is only used for consistency check so only check for ARMv6T2/ARMv7
     in Thumb mode.
     Pre-ARMv6T2, only branch Thumb instructions can be conditional.  */
  Target_CheckCPUFeature (kCPUExt_v6T2, true);

  assert(state->isThen[0]);
  uint32_t cc = state->cc >> (28 - 4);
  uint32_t mask = 0x8;
  for (unsigned cond = state->maxIdx; cond != 1; --cond)
    mask = ((((cc & 0x10) == 0) ^ state->isThen[cond - 1]) << 3) | (mask >> 1);
  Put_InsWithOffset (state->areaCurIdx, 2, 0xBF00 | cc | mask);
}


/**
 * Implements IT.
 *   IT["T" | "E" ["T" | "E" ["T" | "E"]]][<q>] <firstcond>
 */
bool
m_it (bool doLowerCase)
{
  IT_State_t newIT =
    {
      .isThen = { true, false, false, false },
      .maxIdx = 1,
      .curIdx = 0,
      .implicitIT = false,
      /* .cc = ..., */
      .areaCurIdx = areaCurrentSymbol->area->curIdx,
      .fileName = FS_GetCurFileName(),
      .lineNum = FS_GetCurLineNumber()
    };

  for (; newIT.maxIdx != 4; ++newIT.maxIdx)
    {
      if (Input_Match (doLowerCase ? 't' : 'T', false))
	newIT.isThen[newIT.maxIdx] = true;
      else if (Input_Match (doLowerCase ? 'e' : 'E', false))
	newIT.isThen[newIT.maxIdx] = false;
      else
	break;
    }
  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  if (instrWidth == eInstrWidth_Enforce32bit)
    Error (ErrorWarning, "Wide instruction qualifier for Thumb is not possible");

  Input_SkipWS ();
  newIT.cc = Option_GetCCodeIfThere (false); /* Condition code is an argument
    here, so always uppercase.  */
  if (newIT.cc == kOption_NotRecognized)
    {
      Error (ErrorError, "Unrecognized or missing condition code");
      return false;
    }

  Area *curAreaP = areaCurrentSymbol->area;
  if (curAreaP->it.curIdx != curAreaP->it.maxIdx)
    {
      /* We have a pending IT block.  */
      Error (ErrorError, "There is a pending IT block");
      Error_Line (curAreaP->it.fileName, curAreaP->it.lineNum, ErrorError, "note: Pending IT block started here");
    }

  /* We've already gave a warning about the use of NV.
     Here, warn about using AL and having numCond non-zero.  */
  if (newIT.cc == AL && newIT.maxIdx != 1)
    Error (ErrorWarning, "Use of AL condition with one or more Then/Else arguments is UNPREDICTABLE");

  curAreaP->it = newIT;
  IT_EmitIT (&curAreaP->it, State_GetInstrType () != eInstrType_ARM);

  return false;
}


/**
 * Called to initialize the IT state data located in each Area structure.
 */
void
IT_InitializeState (IT_State_t *state)
{
  memset (state, 0, sizeof (IT_State_t));
  state->cc = AL;
}


static void
IT_StartImplicitIT (IT_State_t *itStateP, uint32_t cc)
{
  const IT_State_t newIT =
    {
      .isThen = { true, false, false, false },
      .maxIdx = 1,
      .curIdx = 1,
      .implicitIT = true,
      .cc = cc,
      .areaCurIdx = areaCurrentSymbol->area->curIdx,
      .fileName = FS_GetCurFileName(),
      .lineNum = FS_GetCurLineNumber()
    };
  *itStateP = newIT;
}


/**
 * To be called for each ARM and Thumb instruction with its condition code
 * and whether it is ARM or Thumb instruction.
 * When we have a pending explicit IT block, the condition code will be
 * verified.
 * When we don't have a pending explicit IT block and it is a Thumb
 * instruction with a non-AL condition code, an implicit IT block is
 * generated and an IT instruction is emited.
 * If it's an ARM instruction, no IT instruction is emited.
 * \param cc Condition code.
 * \param enforceLast When true, makes sure that this is going to be the
 * last instruction for the current IT block (if any).  When it is an
 * explicit pending IT block and not the last instruction in that block,
 * an error is given.
 * \param isThumb When true when current instruction is a Thumb instruction.
 */
void
IT_ApplyCond (uint32_t cc, bool enforceLast, bool isThumb)
{
  cc &= NV; /* Filter out the condition codes.  */
  IT_State_t *itStateP = &areaCurrentSymbol->area->it;
  assert (itStateP->curIdx <= itStateP->maxIdx);
  if (itStateP->curIdx != itStateP->maxIdx)
    {
      /* Pending explicit IT block.  */
      assert (!itStateP->implicitIT);
      if (isThumb && enforceLast && itStateP->curIdx + 1 != itStateP->maxIdx)
	{
	  Error (ErrorError, "Not having this instruction as last in pending IT block is UNPREDICTABLE");
	  Error_Line (itStateP->fileName, itStateP->lineNum, ErrorError, "note: Pending IT block started here");
	}
      else
	{
	  uint32_t expectedCC = itStateP->cc ^ (!itStateP->isThen[itStateP->curIdx] << 28);
	  if (cc != expectedCC)
	    {
	      Error (ErrorError, "Condition code does not match with pending IT block");
	      Error_Line (itStateP->fileName, itStateP->lineNum, ErrorError, "note: Pending IT block started here");
	    }
	}
      itStateP->curIdx++;
    }
  else if (itStateP->implicitIT && itStateP->maxIdx != 4)
    {
      /* An implicit IT block which still can be extended.
         If it is not Thumb anymore, we've switched to ARM code while we still
         having an implicit IT block.  */
      if (!isThumb || cc == AL)
	IT_InitializeState (itStateP);
      else
	{
	  if (cc == itStateP->cc)
	    {
	      /* Add 'Then' case.  */
	      itStateP->isThen[itStateP->curIdx] = true;
	      itStateP->curIdx++;
	      itStateP->maxIdx++;
	    }
	  else if (cc == (itStateP->cc ^ (1<<28)))
	    {
	      /* Add 'Else' case.  */
	      itStateP->isThen[itStateP->curIdx] = false;
	      itStateP->curIdx++;
	      itStateP->maxIdx++;
	    }
	  else
	    IT_StartImplicitIT (itStateP, cc);
	  IT_EmitIT (itStateP, true);
	  if (enforceLast)
	    {
	      itStateP->implicitIT = false;
	      itStateP->curIdx = itStateP->maxIdx = 4; /* Finish implicit IT block prematurely.  */
	    }
	}
    }
  else if (isThumb && cc != AL)
    {
      /* Start a new implicit IT block.  */
      IT_StartImplicitIT (itStateP, cc);
      IT_EmitIT (itStateP, true);
      if (enforceLast)
	{
	  itStateP->implicitIT = false;
	  itStateP->curIdx = itStateP->maxIdx = 4; /* Finish implicit IT block prematurely.  */
	}
    }
}


/**
 * \return true when we have an explicit IT block which is still active and
 * given <cc> value fits in it.  Also true for an implicit IT block which
 * can still be extended using the same <cc> code.
 */
bool
IT_CanExtendBlock (uint32_t cc)
{
  assert ((cc & ~NV) == 0 && cc != AL);
  const IT_State_t *itStateP = &areaCurrentSymbol->area->it;
  assert (itStateP->curIdx <= itStateP->maxIdx);
  return (itStateP->curIdx != itStateP->maxIdx && cc == (itStateP->cc ^ (!itStateP->isThen[itStateP->curIdx] << 28)))
           || (itStateP->implicitIT && itStateP->maxIdx != 4 && (cc == itStateP->cc || cc == (itStateP->cc ^ (1<<28))));
}
