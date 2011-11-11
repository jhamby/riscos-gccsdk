/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2011 GCCSDK Developers
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
 * whileif.c
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "code.h"
#include "commands.h"
#include "decode.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "input.h"
#include "value.h"
#include "whileif.h"

#ifdef DEBUG
//#  define DEBUG_WHILEIF
#endif

typedef struct
{
  int lineno;		/* Line number where IF is located.  */
} IfBlock_t;

typedef struct
{
  int lineno;		/* Line number where WHILE is located.  */
  union
    {
      off_t offsetFile; /* Only valid when gCurPObjP->type == POType_eFile */
      const char *offsetMacro; /* Only valid when gCurPObjP->type == POType_eMacro */
    } restoreData;
} WhileBlock_t;

typedef enum
{
  WhileIf_eIsIf,
  WhileIf_eIsWhile
} WhileIf_e;

/* Structure referred in PObject.  */
typedef struct
{
  WhileIf_e Tag;
  union
    {
      IfBlock_t If;
      WhileBlock_t While;
    } Data;
} WhileIf_t;

#define kMAX_WHILEIF_BLOCKS	128
static WhileIf_t oWhileIfs[kMAX_WHILEIF_BLOCKS];

typedef enum
{
  eSkipToElseElifOrEndif, /* Go to matching ELSE, ELIF or ENDIF and enable assembling (with ELIF, only when its argument is {TRUE}).  */
  eSkipToEndif, /* Go to matching ENDIF and enable assembling.  Ignore matching ELIF.  At matching ELSE, switch to ToDo_eSkipToEndifStrict.  */
  eSkipToEndifStrict /* Go to matching ENDIF (no matching ELSE, ELIF are allowed) and enable assembling.  */
} IfSkip_eToDo;

static bool If_Skip (IfSkip_eToDo toDo);
static bool While_Skip (void);
static bool While_Rewind (void);

/**
 * Skip following assembler lines.  And this because of
 * either a failed '[' / 'IF' test, so skip until we find the corresponding
 * matching '|', 'ELSE', 'ELIF', ']' or 'ENDIF',
 * either we had a successful '[' / 'IF' test and we now reached the '|',
 * 'ELSE' or 'ELIF' part.
 * \param toDo Specifies what to do when encountering matching '|', 'ELSE',
 * 'ELIF', ']' and 'ENDIF'.
 */
static bool
If_Skip (IfSkip_eToDo toDo)
{
  /* We will now skip input lines until a matching '|', 'ELSE', 'ELIF', ']' or
    'ENDIF'.  This means we have to do the final decode check ourselves
    for the current line.  */
  decode_finalcheck ();

  int nested = 0;
  while (Input_NextLine (eVarSubstNoWarning))
    {
      /* Ignore blank lines and comments.  */
      if (Input_IsEolOrCommentStart ())
	continue;

      /* Check for label and skip it.
         Make special exception for '$' starting labels, i.e. macro arguments.  */
      bool labelPresent;
      if (isspace ((unsigned char)inputLook ()))
	labelPresent = false;
      else if (inputLook () == '$')
	{
	  size_t len;
	  (void) inputSymbol (&len, '\0');
	  if (Input_Match ('.', false))
	    (void) inputSymbol (&len, '\0');
	  labelPresent = true;
	}
      else
	labelPresent = Lex_SkipDefiningLabel ();
      skipblanks ();

      /* Check for '[', '|', ']', 'IF', 'ELSE', 'ELIF', 'ENDIF'.  */
      enum { t_if, t_else, t_elif, t_endif } toktype;
      if (Input_MatchKeyword ("["))
	toktype = t_if;
      else if (Input_MatchKeyword ("|"))
	toktype = t_else;
      else if (Input_MatchKeyword ("]"))
	toktype = t_endif;
      else if (Input_MatchKeyword ("IF"))
	toktype = t_if;
      else if (Input_MatchKeyword ("ELSE"))
	toktype = t_else;
      else if (Input_MatchKeyword ("ELIF"))
	toktype = t_elif;
      else if (Input_MatchKeyword ("ENDIF"))
	toktype = t_endif;
      else
	continue;

      if (toktype != t_if && toktype != t_elif)
	{
          skipblanks ();
          if (!Input_IsEolOrCommentStart ())
	    error (ErrorError, "Spurious characters after %s token", toktype == t_else ? "ELSE" : "ENDIF");
	}

      if (labelPresent)
	error (ErrorWarning, "Label not allowed here - ignoring");

      switch (toktype)
	{
	  case t_if:
	    nested++;
	    break;

	  case t_else:
	    if (nested == 0)
	      {
		/* Matching.  */
		switch (toDo)
		  {
		    case eSkipToElseElifOrEndif:
		      return false;
		    case eSkipToEndif:
		      toDo = eSkipToEndifStrict; /* From now on, we only expect matching ENDIF (no other matching ELSE, nor matching ELIF).  */
		      break;
		    case eSkipToEndifStrict:
		      error (ErrorError, "Spurious '|' or 'ELSE' is being ignored");
		      break;
		  }
	      }
	    break;

	  case t_elif:
	    if (nested == 0)
	      {
		switch (toDo)
		  {
		    case eSkipToElseElifOrEndif:
		      {
			const Value *flag = exprBuildAndEval (ValueBool);
			bool skipToElseElifOrEndIf;
			if (flag->Tag != ValueBool)
			  {
			    error (ErrorError, "ELIF expression must be boolean (treating as true)");
			    skipToElseElifOrEndIf = false;
			  }
			else
			  skipToElseElifOrEndIf = !flag->Data.Bool.b;
			if (!skipToElseElifOrEndIf)
			  return false;
			break;
		      }
		    case eSkipToEndif:
		      break;
		    case eSkipToEndifStrict:
		      error (ErrorError, "Spurious 'ELIF' is being ignored");
		      break;
		  }
	      }
	    break;

	  case t_endif:
	    if (nested-- == 0)
	      return c_endif ();
	    break;
	}
    }
  /* We reached the end of the current parsing object without finding a matching
     '|', 'ELSE', 'ELIF', ']' nor 'ENDIF'.
     There is no need to given an error about that because this condition
     has already been flagged as an error in FS_PopIfWhile().  */

  /* In order not to confuse decode_finalcheck(), inject an empty
     (rest of the) line.  */
  Input_ThisInstead ("");

  return false;
}

/**
 * Implements '[' and 'IF'.
 * Only called from decode().
 */
bool
c_if (void)
{
  if (gCurPObjP->whileIfCurDepth + 1 == kMAX_WHILEIF_BLOCKS)
    {
      error (ErrorError, "Too many nested WHILE/IFs");
      return false;
    }

  WhileIf_t *whileIfP = &oWhileIfs[gCurPObjP->whileIfCurDepth++];
  whileIfP->Tag = WhileIf_eIsIf;
  whileIfP->Data.If.lineno = gCurPObjP->lineNum;

  const Value *flag = exprBuildAndEval (ValueBool);
  bool skipToElseElifOrEndIf;
  if (flag->Tag != ValueBool)
    {
      error (ErrorError, "IF expression must be boolean (treating as true)");
      skipToElseElifOrEndIf = false;
    }
  else
    skipToElseElifOrEndIf = !flag->Data.Bool.b;

  if (skipToElseElifOrEndIf)
    return If_Skip (eSkipToElseElifOrEndif);

  return false;
}


/**
 * Implements '|' and 'ELSE'.
 * The previous '[' or 'IF' evaluated to {TRUE} and we're now about to enter
 * the '|' or 'ELSE' clause which we have to ignore.
 * The difference between c_else() and c_elif() is that the latter has an
 * argument we should ignore.
 * Only called from decode() and c_elif().
 */
bool
c_else (void)
{
  if (gCurPObjP->whileIfCurDepth == gCurPObjP->whileIfStartDepth)
    error (ErrorError, "Mismatched | or ELSE, no matching IF found");
  else
    {
      if (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsWhile)
	{
	  error (ErrorError, "Mismatched | or ELSE");
	  errorLine (FS_GetCurFileName(), oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Data.While.lineno,
		     ErrorError, "note: Maybe because of an unmatched WHILE here");
	}
      else
	{
	  assert (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsIf);
	  return If_Skip (eSkipToEndifStrict);
	}
    }
  
  return false;
}

/**
 * Implements 'ELIF'.
 * The previous '[' or 'IF' evaluated to {TRUE} and we're now about to enter
 * the 'ELIF' clause which we have to ignore.
 * The difference between c_else() and c_elif() is that the latter has an
 * argument we should ignore.
 * Only called from decode().
 */
bool
c_elif (void)
{
  /* Skip the argument of ELIF.  Not of interest now.  */
  Input_Rest ();

  if (gCurPObjP->whileIfCurDepth == gCurPObjP->whileIfStartDepth)
    error (ErrorError, "Mismatched ELIF, no matching IF found");
  else
    {
      if (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsWhile)
	{
	  error (ErrorError, "Mismatched ELIF");
	  errorLine (FS_GetCurFileName(), oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Data.While.lineno,
		     ErrorError, "note: Maybe because of an unmatched WHILE here");
	}
      else
	{
	  assert (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsIf);
	  return If_Skip (eSkipToEndif);
	}
    }
  
  return false;
}

/**
 * Implements ']' and 'ENDIF'
 * Called from decode() (the previous lines were being assembled) and
 * If_Skip() (the previous lines were being skipped).
 */
bool
c_endif (void)
{
  if (gCurPObjP->whileIfCurDepth == gCurPObjP->whileIfStartDepth)
    error (ErrorError, "Mismatched ] or ENDIF, no matching IF found");
  else
    {
      if (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsWhile)
	{
	  error (ErrorError, "Mismatched ] or ENDIF");
	  errorLine (FS_GetCurFileName(), oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Data.While.lineno,
		     ErrorError, "note: Maybe because of an unmatched WHILE here");
	}
      else
	{
	  assert (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsIf);
	  --gCurPObjP->whileIfCurDepth;
	}
    }
  
  return false;
}


/**
 * Skip following assembler input lines until we find matching WEND.
 */
static bool
While_Skip (void)
{
  /* We will now skip input lines until a matching 'WEND'.  This means we have
     to do the final decode check ourselves for the current line.  */
  decode_finalcheck ();

  const int startLineNumber = FS_GetCurLineNumber ();

  int nested = 0;
  while (Input_NextLine (eVarSubstNoWarning))
    {
      /* Ignore blank lines and comments.  */
      if (Input_IsEolOrCommentStart ())
	continue;
 
      /* Check for label and skip it.
	 Make special exception for '$' starting labels, i.e. macro arguments.  */
      bool labelPresent;
      if (isspace ((unsigned char)inputLook ()))
	labelPresent = false;
      else if (inputLook () == '$')
	{
	  size_t len;
	  (void) inputSymbol (&len, '\0');
	  if (Input_Match ('.', false))
	    (void) inputSymbol (&len, '\0');
	  labelPresent = true;
	}
      else
	labelPresent = Lex_SkipDefiningLabel ();
      skipblanks ();      

      /* Look for WHILE and WEND.  */
      if (Input_MatchKeyword ("WHILE"))
	{
	  if (labelPresent)
	    error (ErrorWarning, "Label not allowed here - ignoring");
	  ++nested;
	}
      else if (Input_MatchKeyword ("WEND"))
	{
	  if (labelPresent)
	    error (ErrorWarning, "Label not allowed here - ignoring");
	  if (nested-- == 0)
	    {
	      assert (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsWhile);
	      --gCurPObjP->whileIfCurDepth;
	      return false;
	    }
	}
    }

  /* We reached the end of the current parsing object without finding a matching
     'WEND'.  */
  error (ErrorError, "Mismatched WHILE, did you forget WEND");
  errorLine (FS_GetCurFileName(), startLineNumber,
	     ErrorError, "note: WHILE started here");

  /* In order not to confuse decode_finalcheck(), inject an empty
     (rest of the) line.  */
  Input_ThisInstead ("");

  return false;
}


/**
 * Implements WHILE.
 */
bool
c_while (void)
{
  /* Figure out if we're re-evaluating the current WHILE body or not.  */
  bool isReeval;
  assert (gCurPObjP->whileIfCurDepth >= gCurPObjP->whileIfStartDepth);
  if (gCurPObjP->whileIfCurDepth == gCurPObjP->whileIfStartDepth)
    isReeval = false;
  else
    {
      const WhileIf_t *whileIfP = &oWhileIfs[gCurPObjP->whileIfCurDepth - 1];
      if (whileIfP->Tag != WhileIf_eIsWhile)
	isReeval = false;
      else
	isReeval = whileIfP->Data.While.lineno == gCurPObjP->lineNum;
    }
  
  if (!isReeval)
    {
      /* First time this WHILE gets executed, create a WHILE context.  */
      if (gCurPObjP->whileIfCurDepth + 1 == kMAX_WHILEIF_BLOCKS)
	{
	  error (ErrorError, "Too many nested WHILE/IFs");
	  return false;
	}

      WhileIf_t *whileIfP = &oWhileIfs[gCurPObjP->whileIfCurDepth++];
      whileIfP->Tag = WhileIf_eIsWhile;
      whileIfP->Data.While.lineno = gCurPObjP->lineNum;
      switch (gCurPObjP->type)
	{
	  case POType_eFile:
	    whileIfP->Data.While.restoreData.offsetFile = ftell (gCurPObjP->d.file.fhandle) - gCurPObjP->lastLineSize;
	    break;

	  case POType_eMacro:
	    whileIfP->Data.While.restoreData.offsetMacro = gCurPObjP->d.macro.curPtr - gCurPObjP->lastLineSize;
	    break;

	  default:
	    assert (0 && "Unexpected parsable object type");
	    break;
	}
    }

  /* Evaluate expression.  */
  const Value *flag = exprBuildAndEval (ValueBool);
  bool whileExprResult;
  if (flag->Tag != ValueBool)
    {
      error (ErrorError, "WHILE expression must be boolean (treating as false)");
      whileExprResult = false;
    }
  else
    whileExprResult = flag->Data.Bool.b;
#ifdef DEBUG_WHILEIF
  printf("c_while() : expr is <%s>\n", whileExprResult ? "true" : "false");
#endif

  if (!whileExprResult)
    return While_Skip ();

  return false;
}


/**
 * Re-evaluates WHILE expression and prepares body parsing if it needs to be
 * re-executed.
 */
static bool
While_Rewind (void)
{
  assert (gCurPObjP->whileIfCurDepth > gCurPObjP->whileIfStartDepth);
  assert (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsWhile);
  const WhileBlock_t *whileBlockP = &oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Data.While;

  gCurPObjP->lineNum = whileBlockP->lineno - 1;
  switch (gCurPObjP->type)
    {
      case POType_eFile:
	fseek (gCurPObjP->d.file.fhandle, whileBlockP->restoreData.offsetFile, SEEK_SET);
	break;

      case POType_eMacro:
	gCurPObjP->d.macro.curPtr = whileBlockP->restoreData.offsetMacro;
	break;

      default:
	assert (0 && "unrecognised WHILE type");
	break;
    }

  return false;
}


/**
 * Implements WEND.
 */
bool
c_wend (void)
{
  assert (gCurPObjP->whileIfCurDepth >= gCurPObjP->whileIfStartDepth);
  if (gCurPObjP->whileIfCurDepth == gCurPObjP->whileIfStartDepth)
    error (ErrorError, "Mismatched WEND, did you forgot WHILE");
  else if (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsIf)
    {
      error (ErrorError, "Mismatched WEND, did you forgot WHILE");
      errorLine (FS_GetCurFileName(), oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Data.If.lineno,
		 ErrorError, "note: Maybe because of an unmatched IF here");
    }
  else
    {
      assert (oWhileIfs[gCurPObjP->whileIfCurDepth - 1].Tag == WhileIf_eIsWhile);
      /* Re-evaluate WHILE expression.  */
      return While_Rewind ();
    }

  return false;
}


void
FS_PopIfWhile (bool noCheck)
{
  assert (gCurPObjP->whileIfCurDepth >= gCurPObjP->whileIfStartDepth);
  while (gCurPObjP->whileIfCurDepth != gCurPObjP->whileIfStartDepth)
    {
      --gCurPObjP->whileIfCurDepth;
      switch (oWhileIfs[gCurPObjP->whileIfCurDepth].Tag)
	{
	  case WhileIf_eIsIf:
	    if (!noCheck)
	      {
		error (ErrorError, "Unmatched IF/ELSE/ELIF, did you forgot ENDIF");
		errorLine (FS_GetCurFileName(), oWhileIfs[gCurPObjP->whileIfCurDepth].Data.If.lineno,
			   ErrorError, "note: IF started here");
	      }
	    break;

	  case WhileIf_eIsWhile:
	    if (!noCheck)
	      {
		error (ErrorError, "Unmatched WHILE, did you forgot WEND");
		errorLine (FS_GetCurFileName(), oWhileIfs[gCurPObjP->whileIfCurDepth].Data.While.lineno,
			   ErrorError, "note: WHILE started here");
	      }
	    break;

	  default:
	    assert (0 && "Unknown while/if type");
	    break;
	}
    }
}
