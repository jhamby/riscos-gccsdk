/*
 * AS an assembler for ARM
 * Copyright (c) 2012 GCCSDK Developers
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
 * opt.c
 */

#include "config.h"

#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "opt.h"

/* FIXME: We don't make use of these.  */
#define kOpt_TurnOnNormalListing (1<<0)
#define kOpt_TurnOffNormalListing (1<<1)
#define kOpt_PageThrow (1<<2)
#define kOpt_ResetLineNum (1<<3)
#define kOpt_TurnOnSetGblLclListing (1<<4)
#define kOpt_TurnOffSetGblLclListing (1<<5)
#define kOpt_TurnOnMacroExpansionListing (1<<6)
#define kOpt_TurnOffMacroExpansionListing (1<<7)
#define kOpt_TurnOnMacroCallListing (1<<8)
#define kOpt_TurnOffMacroCallListing (1<<9)
#define kOpt_TurnOnPassOneListing (1<<10)
#define kOpt_TurnOffPassOneListing (1<<11)
#define kOpt_TurnOnConditionalDirectivesListing (1<<12)
#define kOpt_TurnOffConditionalDirectivesListing (1<<13)
#define kOpt_TurnOnMENDDirectiveListing (1<<14)
#define kOpt_TurnOffMENDDirectiveListing (1<<15)

#define kOpt_DefaultValue \
  (kOpt_TurnOnNormalListing \
   | kOpt_TurnOnSetGblLclListing \
   | kOpt_TurnOnMacroExpansionListing \
   | kOpt_TurnOnMacroCallListing \
   | kOpt_TurnOnConditionalDirectivesListing)

#define kOpt_MaskOn \
  (kOpt_TurnOnNormalListing \
   | kOpt_TurnOnSetGblLclListing \
   | kOpt_TurnOnMacroExpansionListing \
   | kOpt_TurnOnMacroCallListing \
   | kOpt_TurnOnConditionalDirectivesListing)

unsigned gOpt_DirectiveValue;

void
Opt_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case ePassOne:
      case ePassTwo:
	gOpt_DirectiveValue = kOpt_DefaultValue;
	break;

      case eStartup:
      case eOutput:
	break;
    }
}


/**
 * Implements OPT.
 */
bool
c_opt (void)
{
  const Value *value = exprBuildAndEval (ValueInt);
  if (value->Tag != ValueInt)
    {
      error (ErrorError, "OPT expression must be an integer");
      return false;
    }
  unsigned opt = (unsigned)value->Data.Int.i;

  opt &= ~kOpt_PageThrow; /* FIXME: no support for this.  */
  if (opt & kOpt_ResetLineNum)
    {
      opt &= ~kOpt_ResetLineNum;
      gCurPObjP->lineNum = 0;
    }
  /* Ignore the off bits when corresponding on bits have been set.  */
  opt &= ~((opt & kOpt_MaskOn)<<1);
  if (opt & kOpt_TurnOffMacroExpansionListing)
    opt = (opt & ~(kOpt_TurnOnNormalListing | kOpt_TurnOffNormalListing)) | kOpt_TurnOffNormalListing;
  else if (opt & kOpt_TurnOnMacroExpansionListing)
    opt = (opt & ~(kOpt_TurnOnNormalListing | kOpt_TurnOffNormalListing)) | kOpt_TurnOnNormalListing;
  unsigned forceSet = opt & kOpt_MaskOn;
  unsigned forceClear = opt & (kOpt_MaskOn << 1);
  gOpt_DirectiveValue = (gOpt_DirectiveValue & ~(forceSet | (forceSet<<1) | forceClear | (forceClear>>1)))
                          | forceSet
                          | forceClear;
  return false;
}
