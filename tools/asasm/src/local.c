/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2012 GCCSDK Developers
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
 * local.c
 */

#include "config.h"

#include <string.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "asm.h"
#include "common.h"
#include "error.h"
#include "filestack.h"
#include "lex.h"
#include "local.h"
#include "state.h"

#ifdef DEBUG
//#  define DEBUG_LOCAL
#endif

typedef struct
{
  const char *id;
  unsigned counter;

  const char *fileName;
  unsigned lineNum;
} ROUT_t;

static ROUT_t oLocal_CurROUT;

/* Represents an outstanding forward local label reference.  */
typedef struct Local_OutstandingForward
{
  struct Local_OutstandingForward *nextP; /* Must be first.  */
  const char *fileName;
  unsigned lineNum;
  unsigned counter;

  LocalLabel_eLevel level;
  LocalLabel_eDir dir;
  unsigned label;
  unsigned macroDepth;
} Local_OutstandingForward;

/* We store the local labels for each macro level separately where
   they are defined in.  */
static Local_Label_t *oLocal_LabelNumP[PARSEOBJECT_STACK_SIZE][32];

static Local_OutstandingForward *oLocal_OutstandingForwardP;
static unsigned oLocal_OutstandingForwardCounter;

/* Parameters: AREA ptr, rout counter, macro level, label number, instance number.  */
static const char oLocal_IntLabelFormat[] = kIntLabelPrefix "Local$$%p$$%u$$%u$$%08u$$%u";

/* Parameters: label number, ever increasing value.  */
static const char oLocal_IntFwdLabelFormat[] = kIntLabelPrefix "FwdLocal$$%08i$$%i";

static void Local_ReportMissingFwdLabel (const Local_OutstandingForward *fwdLocalP);
static void Local_FinishPhaseOrROUT (void);
static void Local_ResetLabels (void);


void
Local_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case eStartup:
      case ePassOne:
	break;

      case ePassTwo:
      case eOutput:
	{
	  Local_FinishPhaseOrROUT ();
	  Local_ResetLabels ();
	  oLocal_OutstandingForwardCounter = 0;

	  free ((void *)oLocal_CurROUT.id);
	  oLocal_CurROUT.id = NULL;
	  oLocal_CurROUT.counter = 0;
	  oLocal_CurROUT.fileName = NULL;
	  oLocal_CurROUT.lineNum = 0;
	  break;
	}
    }
}


static void
Local_ReportMissingFwdLabel (const Local_OutstandingForward *fwdLocalP)
{
  const char *dirStr = fwdLocalP->dir == eForward ? "f" : "";
  const char *levelStr = fwdLocalP->level == eThisLevelOnly ? "t" : fwdLocalP->level == eAllLevels ? "a" : ""; 
  errorLine (fwdLocalP->fileName, fwdLocalP->lineNum, ErrorError, "Missing local label %%%s%s%i", dirStr, levelStr, fwdLocalP->label);
}


/**
 * Called at the end of a phase or via ROUT.
 */
static void
Local_FinishPhaseOrROUT (void)
{
  Local_OutstandingForward *prevFwdLocalP = (Local_OutstandingForward *)&oLocal_OutstandingForwardP;
  while (prevFwdLocalP->nextP != NULL)
    {
      Local_OutstandingForward * const fwdLocalP = prevFwdLocalP->nextP;

      Local_ReportMissingFwdLabel (fwdLocalP);

      prevFwdLocalP->nextP = fwdLocalP->nextP;
      free (fwdLocalP);
    }
}


/**
 * Called at the end of each macro invocation.  We check for unresolved
 * forward local labels.
 */
void
Local_FinishMacro (bool noCheck)
{
  unsigned macroDepth = FS_GetMacroDepth ();
  Local_OutstandingForward *prevFwdLocalP = (Local_OutstandingForward *)&oLocal_OutstandingForwardP;
  while (prevFwdLocalP->nextP != NULL)
    {
      Local_OutstandingForward * const fwdLocalP = prevFwdLocalP->nextP;

      assert (fwdLocalP->level != eThisLevelOnly || fwdLocalP->macroDepth <= macroDepth); 
      if (fwdLocalP->level == eThisLevelOnly
          && fwdLocalP->macroDepth == macroDepth)
	{
	  if (!noCheck)
	    Local_ReportMissingFwdLabel (fwdLocalP);
	  
	  prevFwdLocalP->nextP = fwdLocalP->nextP;
	  free (fwdLocalP);
	}
      else
	prevFwdLocalP = prevFwdLocalP->nextP;
    }
}


Local_Label_t *
Local_DefineLabel (unsigned labelNum)
{
  const unsigned i = labelNum % (sizeof (oLocal_LabelNumP[0]) / sizeof (oLocal_LabelNumP[0][0]));
  const unsigned macroDepth = FS_GetMacroDepth ();
  Local_Label_t *prevLblP, *lblP;
  for (prevLblP = (Local_Label_t *)&oLocal_LabelNumP[macroDepth][i], lblP = prevLblP->nextP;
       lblP != NULL && lblP->num != labelNum;
       prevLblP = lblP, lblP = prevLblP->nextP)
    /* */;
  if (lblP == NULL)
    {
      lblP = prevLblP->nextP = malloc (sizeof (Local_Label_t));
      if (lblP == NULL)
	errorOutOfMem ();
      lblP->nextP = NULL;
      lblP->num = labelNum;
      lblP->instance = 0;
    }
  
  /* Check if this definition of a local variable can not be used to
     resolve one of our pending forward local label references.  */
  Local_OutstandingForward *prevFwdLocalP = (Local_OutstandingForward *)&oLocal_OutstandingForwardP;
  while (prevFwdLocalP->nextP != NULL)
    {
      Local_OutstandingForward * const fwdLocalP = prevFwdLocalP->nextP;

      if (fwdLocalP->label == labelNum
          && ((fwdLocalP->level == eThisLevelOnly && fwdLocalP->macroDepth == macroDepth)
	      || fwdLocalP->level == eAllLevels
	      || (fwdLocalP->level == eThisLevelAndHigher && fwdLocalP->macroDepth >= macroDepth)))
	{
	  char fwdSym[256];
	  int r = snprintf (fwdSym, sizeof (fwdSym), oLocal_IntFwdLabelFormat, fwdLocalP->label, fwdLocalP->counter);
	  assert (r >= 0 && (size_t)r < sizeof (fwdSym));
	  const Lex keyLex = Lex_Id (fwdSym, strlen (fwdSym));
	  Symbol *keySymbol = Symbol_Get (&keyLex);
#ifdef DEBUG_LOCAL
	  const char *levelStr = (fwdLocalP->level == eThisLevelOnly) ? "t" : (fwdLocalP->level == eAllLevels) ? "a" : "";
	  const char *dirStr = "f"; // (dir == eBackward) ? "b" : (dir == eForward) ? "f" : "";
	  printf ("Define forward ref: %.*s %u : %%%s%s%d : %s\n", 12, FS_GetCurFileName (), FS_GetCurLineNumber (),
		  dirStr, levelStr, labelNum, fwdSym);
#endif

	  char lblSym[256];
	  Local_CreateSymbol (lblP, macroDepth, true, lblSym, sizeof (lblSym));
	  const Lex valueLex = Lex_Id (lblSym, strlen (lblSym));
	  Symbol *valueSymbol = Symbol_Get (&valueLex);
	  const Value valueValue = Value_Symbol (valueSymbol, 1, 0);

          bool err = Symbol_Define (keySymbol, SYMBOL_NO_EXPORT, &valueValue);
	  assert (!err);
	  
	  prevFwdLocalP->nextP = fwdLocalP->nextP;
	  free (fwdLocalP);
	}
      else
	prevFwdLocalP = prevFwdLocalP->nextP;
    }

  return lblP;
}

void Local_CreateSymbolForOutstandingFwdLabelRef (char *buf, size_t bufSize,
						  LocalLabel_eLevel level,
						  LocalLabel_eDir dir,
						  unsigned label)
{
  Local_OutstandingForward *fwdLocalP = malloc (sizeof (Local_OutstandingForward));
  if (fwdLocalP == NULL)
    errorOutOfMem ();

  fwdLocalP->nextP = oLocal_OutstandingForwardP; 
  fwdLocalP->fileName = FS_GetCurFileName ();
  fwdLocalP->lineNum = FS_GetCurLineNumber ();
  fwdLocalP->counter = oLocal_OutstandingForwardCounter++;

  fwdLocalP->level = level;
  fwdLocalP->dir = dir;
  fwdLocalP->label = label;
  fwdLocalP->macroDepth = FS_GetMacroDepth ();

  oLocal_OutstandingForwardP = fwdLocalP;

  int r = snprintf (buf, bufSize, oLocal_IntFwdLabelFormat, fwdLocalP->label, fwdLocalP->counter);
  assert (r >= 0 && (size_t)r < bufSize);
#ifdef DEBUG_LOCAL
  const char *levelStr = (level == eThisLevelOnly) ? "t" : (level == eAllLevels) ? "a" : "";
  const char *dirStr = (dir == eBackward) ? "b" : (dir == eForward) ? "f" : "";
  printf ("Create forward ref: %.*s %u : %%%s%s%d : %s\n", 12, FS_GetCurFileName (), FS_GetCurLineNumber (),
	  dirStr, levelStr, label, buf);
#endif
}


Local_Label_t *
Local_GetLabel (unsigned macroDepth, unsigned num)
{
  unsigned i = num % (sizeof (oLocal_LabelNumP[0]) / sizeof (oLocal_LabelNumP[0][0]));
  Local_Label_t *lblP;
  for (lblP = oLocal_LabelNumP[macroDepth][i];
       lblP != NULL && lblP->num != num;
       lblP = lblP->nextP)
    /* */;
  return lblP;
}


/**
 * Create symbol string for given local label.
 * \param next false for previous local label, true for next local label
 * with label value Local_Label_t::num.
 */
void
Local_CreateSymbol (Local_Label_t *lblP, unsigned macroDepth, bool next, char *buf, size_t bufSize)
{
  assert (next || lblP->instance > 0);
  int r = snprintf (buf, bufSize, oLocal_IntLabelFormat, (void *)areaCurrentSymbol,
                    oLocal_CurROUT.counter, macroDepth, lblP->num, next ? lblP->instance : lblP->instance - 1);
  assert (r >= 0 && (size_t)r < bufSize);
}

static void
Local_ResetLabels (void)
{
  for (unsigned macroDepth = 0; macroDepth != sizeof (oLocal_LabelNumP) / sizeof (oLocal_LabelNumP[0]); ++macroDepth)
    {
      for (unsigned i = 0; i != sizeof (oLocal_LabelNumP[0]) / sizeof (oLocal_LabelNumP[0][0]); ++i)
	{
	  for (const Local_Label_t *lblP = oLocal_LabelNumP[macroDepth][i]; lblP != NULL; /* */)
	    {
	      const Local_Label_t *nextLblP = lblP->nextP;
	      free ((void *)lblP);
	      lblP = nextLblP;
	    }
	}
    }
  memset (oLocal_LabelNumP, 0, sizeof (oLocal_LabelNumP));
}


/**
 * \param fileNamePP Will be filled in with ptr to filename of last ROUT parsed.
 * When NULL, no ROUT has been seen so far.
 * \param lineNumP Will be filled in with linenumber of last ROUT parsed.
 */
const char *
Local_GetCurROUTId (const char **fileNamePP, unsigned *lineNumP)
{
  *fileNamePP = oLocal_CurROUT.fileName;
  *lineNumP = oLocal_CurROUT.lineNum;
  return oLocal_CurROUT.id;
}


/**
 * Implements ROUT.
 */
bool
c_rout (const Lex *label)
{
  Local_FinishPhaseOrROUT ();
  Local_ResetLabels ();

  char *newROUTId;
  switch (label->tag)
    {
      case LexNone:
	newROUTId = NULL;
	break;

      case LexId:
	{
	  unsigned alignValue = State_GetInstrType () == eInstrType_ARM ? 4 : 2;
	  size_t curIdx = Area_AlignArea (areaCurrentSymbol, alignValue, "instruction");
	  ASM_DefineLabel (label, curIdx, false);
	  newROUTId = strndup (label->Data.Id.str, label->Data.Id.len);
	  break;
	}

      case LexLocalLabel:
	error (ErrorError, "Local label is not allowed here");
	return false;

      default:
	assert (0);
	break;
    }

  free ((void *)oLocal_CurROUT.id);
  oLocal_CurROUT.id = newROUTId;
  oLocal_CurROUT.counter++;
  oLocal_CurROUT.fileName = FS_GetCurFileName ();
  oLocal_CurROUT.lineNum = FS_GetCurLineNumber ();
  return false;
}


bool
Local_IsLocalLabel (const char *s)
{
  return !memcmp (s, kIntLabelPrefix, sizeof (kIntLabelPrefix)-1);
}


#ifdef DEBUG
void
Local_DumpAll (void)
{
}
#endif