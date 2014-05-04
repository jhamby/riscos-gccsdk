/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2014 GCCSDK Developers
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

#include <ctype.h>
#include <ieee754.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "common.h"
#include "error.h"
#include "filestack.h"
#include "fpu.h"
#include "input.h"
#include "lex.h"
#include "local.h"
#include "main.h"
#include "opt.h"
#include "symbol.h"
#include "state.h"
#include "targetcpu.h"


static Lex
Lex_None (void)
{
  const Lex result = { .tag = LexNone };
  return result;
}


static Lex
Lex_Bool (bool b)
{
  const Lex result =
    {
      .tag = LexBool,
      .Data.Bool.value = b
    };
  return result;
}


static Lex
Lex_Int (int i)
{
  const Lex result =
    {
      .tag = LexInt,
      .Data.Int.value = i
    };
  return result;
}


/**
 * \param str NUL terminated string.
 */
static Lex
Lex_String (const char *str, size_t len, bool ownership)
{
  const Lex result =
    {
      .tag = LexString,
      .Data.String = { .str = str, .len = len, .owns = ownership }
    };
  return result;
}


static Lex
Lex_Float (double dbl)
{
  const Lex result =
    {
      .tag = LexFloat,
      .Data.Float.value = dbl
    };
  return result;
}


Lex
Lex_Id (const char *str, size_t strLen)
{
  const Lex result =
    {
      .tag = LexId,
      .Data.Id = { .str = str, .len = strLen }
    };
  return result;
}


Lex
Lex_GetIDNoError (void)
{
  Input_SkipWS ();

  size_t idLen;
  const char *id = Input_Symbol (&idLen);
  return id ? Lex_Id (id, idLen) : Lex_None ();
}


Lex
Lex_GetID (void)
{
  Lex result = Lex_GetIDNoError ();
  if (result.tag != LexId)
    Error (ErrorError, "Missing or wrong identifier");
  return result;
}


/**
 * Try to parse ROUT identifier, if there is one, check if it is matching
 * the current ROUT one.
 * \return true in case of a ROUT identifier mismatch, false otherwise.
 */
static bool
Lex_CheckForROUTMismatch (const char *rout, size_t routLen)
{
  const char *fileP;
  unsigned lineNum;
  const char *curROUTId = Local_GetCurROUTId (&fileP, &lineNum);
  if (routLen
      && (curROUTId == NULL
          || (memcmp (curROUTId, rout, routLen) || curROUTId[routLen] != '\0')))
    {
      if (curROUTId == NULL)
	Error (ErrorError, "Local label can not have a routine name %.*s here", (int)routLen, rout);
      else
	Error (ErrorError, "Local label with routine name %.*s does not match with current routine name %s", (int)routLen, rout, curROUTId);
      if (fileP != NULL)
	Error_Line (fileP, lineNum, ErrorError, "note: Last ROUT was here");
      return true;
    }
  return false;
}


/**
 * \return UINT_MAX when it wasn't able to read a local label, otherwise a local
 * label value.
 *
 * Very similar to Lex_ReadDefiningLocalLabel, Lex_GetDefiningLabel and
 * Lex_SkipDefiningLabel.
 */
static unsigned
Lex_ReadReferringLocalLabel (void)
{
  if (!isdigit ((unsigned char)Input_Look ()))
    {
      Error (ErrorError, "Missing local label number");
      return UINT_MAX;
    }
  unsigned label = (unsigned char)Input_GetC () - '0';
  while (isdigit ((unsigned char)Input_Look ()))
    {
      unsigned next_label = 10*label + (unsigned char)Input_GetC () - '0';
      if (next_label < label)
	{
	  Error (ErrorError, "Local label overflow");
	  return UINT_MAX;
	}
      label = next_label;
    }

  /* If a routinename is given, check if thats the one given with ROUT.  */
  size_t routLen;
  const char *rout = Input_Symbol2 (&routLen, '\0');
  if (Lex_CheckForROUTMismatch (rout, routLen))
    return UINT_MAX;

  return label;
}


/**
 * \return UINT_MAX when it wasn't able to read a local label, otherwise a local
 * label value.
 *
 * Very similar to Lex_ReadReferringLocalLabel, Lex_GetDefiningLabel and
 * Lex_SkipDefiningLabel.
 */
static unsigned
Lex_ReadDefiningLocalLabel (const char *lblStrP, size_t lblStrSize)
{
  assert (isdigit ((unsigned char)*lblStrP) && lblStrSize);
  unsigned label = (unsigned char)*lblStrP++ - '0';
  --lblStrSize;
  while (lblStrSize && isdigit ((unsigned char)*lblStrP))
    {
      unsigned next_label = 10*label + (unsigned char)*lblStrP++ - '0';
      if (next_label < label)
	{
	  Error (ErrorError, "Local label overflow");
	  return UINT_MAX;
	}
      label = next_label;
      --lblStrSize;
    }

  /* Check if routine name (when given) matches the one given with last
     ROUT.  */
  if (Lex_CheckForROUTMismatch (lblStrP, lblStrSize))
    return UINT_MAX;
  return label;
}


static Lex
Lex_MakeReferringLocalLabel (LocalLabel_eDir dir,
			    LocalLabel_eLevel level)
{
  unsigned label = Lex_ReadReferringLocalLabel ();
  if (label == UINT_MAX)
    return Lex_None ();

  Local_Label_t *lblP;
  unsigned macroDepth;
  switch (dir)
    {
      case eBackward:
      case eBackwardThenForward:
	{
	  macroDepth = FS_GetMacroDepth ();
	  switch (level)
	    {
	      case eThisLevelOnly:
		lblP = Local_GetLabel (macroDepth, label);
		break;

	      case eAllLevels:
		{
		  const unsigned macroDepthCopy = macroDepth;
		  while ((lblP = Local_GetLabel (macroDepth, label)) == NULL
			 && ++macroDepth != PARSEOBJECT_STACK_SIZE)
		    /* */;
		  if (lblP != NULL)
		    break;
		  macroDepth = macroDepthCopy;
		  /* Fall through.  */
		}
	      case eThisLevelAndHigher:
		while ((lblP = Local_GetLabel (macroDepth, label)) == NULL
		       && macroDepth)
		  --macroDepth;
		break;
	    }
	  break;
	}

      case eForward:
	lblP = NULL;
	break;
    }
  char id[256];
  if (lblP == NULL)
    {
      if (dir == eBackward)
	{
	  const char *levelStr = level == eThisLevelOnly ? "t" : level == eAllLevels ? "a" : ""; 
	  Error_Abort ("Missing local label %%b%s%i", levelStr, label);
	  return Lex_None ();
	}
      Local_CreateSymbolForOutstandingFwdLabelRef (id, sizeof (id),
						   level, dir, label);
    }
  else
    Local_CreateSymbol (lblP, macroDepth, false, id, sizeof (id));

  /* Trick: put our local label as a symbol in our symbol hash table.  */
  const Symbol *lclLabelSymP = Symbol_Get (id, strlen (id));
  return Lex_Id (lclLabelSymP->str, lclLabelSymP->len);
}


/**
 * \return LexNone (no or bad label), LexLocalLabel (local label) or
 * LexId (symbol label).
 *
 * Very similar to Lex_ReadReferringLocalLabel, Lex_ReadDefiningLocalLabel,
 * and Lex_SkipDefiningLabel.
 */
Lex
Lex_GetDefiningLabel (void)
{
  if (isdigit ((unsigned char)Input_Look ()))
    {
      /* Looks like this is a local label.  We just turn this into a
         LexLocalLabel Lex object and only when it is going to be used as
         a defining label (i.e. Lex_DefineLocalLabel), we'll check if this
	 is a valid local label.  */
      const char *beginLabel = Input_GetMark ();
      while (isdigit ((unsigned char)Input_Look ()))
	(void) Input_GetC ();
      /* Possibly followed by a ROUT identifier.  */
      size_t routLen;
      (void) Input_Symbol2 (&routLen, '\0');

      Lex result =
	{
	  .tag = LexLocalLabel,
	  .Data.LocalLabel.len = Input_GetMark () - beginLabel,
	  .Data.LocalLabel.str = beginLabel
	};
      return result;
    }

  return Lex_GetID ();
}


/**
 * Tries to turn a LexLocalLabel object into a local label symbol.
 * \return LexId which can be used to create a label symbol.  Can also
 * be LexNone in case of an Error (like malformed local label, or wrong
 * routine name).
 */
Lex
Lex_DefineLocalLabel (const Lex *lexP)
{
  assert (lexP->tag == LexLocalLabel);

  unsigned labelNum = Lex_ReadDefiningLocalLabel (lexP->Data.LocalLabel.str, lexP->Data.LocalLabel.len);
  if (labelNum == UINT_MAX)
    return Lex_None ();
  
  Local_Label_t *lblP = Local_DefineLabel (labelNum);
  char id[256];
  Local_CreateSymbol (lblP, FS_GetMacroDepth (), true, id, sizeof (id));
  lblP->instance++;

  /* Trick: put our local label as a symbol in our symbol hash table.  */
  const Symbol *lclLabelSymP = Symbol_Get (id, strlen (id));
  return Lex_Id (lclLabelSymP->str, lclLabelSymP->len);
}


/**
 * Used to skip defining labels in failed IF/WHILE bodies.
 * \return true when label got read
 *
 * Very similar to Lex_ReadReferringLocalLabel, Lex_ReadDefiningLocalLabel
 * and Lex_GetDefiningLabel.
 */
bool
Lex_SkipDefiningLabel (void)
{
  if (isdigit ((unsigned char)Input_Look ()))
    {
      /* Looks like this is a local label.  */
      do
	{
	  (void) Input_GetC ();
	} while (isdigit ((unsigned char)Input_Look ()));

      size_t len;
      (void) Input_Symbol2 (&len, '\0');
      return true;
    }

  return Lex_GetID ().tag != LexNone;
}

#define FINISH_STR(string, Op, Pri)	\
  if (Input_MatchString (string))	\
    {					\
      lex->Data.Operator.op = Op;	\
      lex->Data.Operator.pri = Pri;	\
      return;				\
    }

#define FINISH_CHR(Op, Pri)		\
  if (Input_Match (':', false))		\
    {					\
      lex->Data.Operator.op = Op;	\
      lex->Data.Operator.pri = Pri;	\
      return;				\
    }

static void
Lex_GetUnaryOp (Lex *lex)
{
  lex->tag = LexOperator;
  char c1 = Input_GetC ();
  switch (c1)
    {
      case 'B':
	FINISH_STR ("ASE:", eOp_Base, kPrioOp_Unary); /* :BASE: */
	break;

      case 'C':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'C':
		FINISH_STR ("_ENCODING:", eOp_CCEnc, kPrioOp_Unary); /* :CC_ENCODING: */
		break;
	      case 'H':
		FINISH_STR ("R:", eOp_Chr, kPrioOp_Unary); /* :CHR: */
		break;
	    }
	  Input_UnGetC (c2);
	  break;
	}

      case 'D':
	if (Input_MatchString ("EF:")) /* :DEF: */
	  {
	    *lex = Lex_GetPrim ();
	    if (lex->tag == LexId)
	      {
	        /* :DEF: only returns {TRUE} when the symbol is defined and it is
		   not a macro local variable.  */
		const Symbol *symP = Symbol_Find (lex->Data.Id.str, lex->Data.Id.len);
		lex->Data.Bool.value = symP != NULL && !(symP->attr.type & SYMBOL_MACRO_LOCAL);
		lex->tag = LexBool;
		return;
	      }
	    else
	      Error (ErrorError, "Bad operand for :DEF:");
	  }
	break;

      case 'F':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'A':
		FINISH_STR ("TTR:", eOp_FAttr, kPrioOp_Unary); /* :FATTR: */
		break;
	      case 'E':
		FINISH_STR ("XEC:", eOp_FExec, kPrioOp_Unary); /* :FEXEC: */
		break;
	      case 'L':
		FINISH_STR ("OAD:", eOp_FLoad, kPrioOp_Unary); /* :FLOAD: */
		break;
	      case 'S':
		FINISH_STR ("IZE:", eOp_FSize, kPrioOp_Unary); /* :FSIZE: */
		break;
	    }
	  Input_UnGetC (c2);
	  break;
	}
	
      case 'I':
	FINISH_STR ("NDEX:", eOp_Index, kPrioOp_Unary); /* :INDEX: */
	break;

      case 'L':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'E':
		FINISH_STR ("N:", eOp_Len, kPrioOp_Unary); /* :LEN: */
		break;
	      case 'O':
		FINISH_STR ("WERCASE:", eOp_LowerCase, kPrioOp_Unary); /* :LOWERCASE: */
	      case 'N':
		FINISH_STR ("OT:", eOp_LNot, kPrioOp_Unary); /* :LNOT: */
		break;
	    }
	  Input_UnGetC (c2);
	  break;
	}
	
      case 'N':
	FINISH_STR ("OT:", eOp_Not, kPrioOp_Unary); /* :NOT: */
	break;

      case 'R':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'C':
		FINISH_STR ("ONST:", eOp_RConst, kPrioOp_Unary); /* :RCONST: */
		break;
	      case 'E':
		FINISH_STR ("VERSE_CC:", eOp_RevCC, kPrioOp_Unary); /* :REVERSE_CC: */
		break;
	    }
	  Input_UnGetC (c2);
	  break;
	}

      case 'S':
	FINISH_STR ("TR:", eOp_Str, kPrioOp_Unary); /* :STR: */
	break;

      case 'U':
	FINISH_STR ("PPERCASE:", eOp_UpperCase, kPrioOp_Unary); /* :UPPERCASE: */
	break;
    }
  Input_UnGetC (c1);

  /* Failed to tokenize.  */
  lex->tag = LexNone;
}


static void
Lex_GetBinaryOp (Lex *lex)
{
  lex->tag = LexOperator;
  char c1 = Input_GetC ();
  switch (c1)
    {
      case 'A':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'N':
		FINISH_STR ("D:", eOp_And, kPrioOp_AddAndLogical); /* :AND: */
		break;
	      case 'S':
		FINISH_STR ("R:", eOp_ASR, kPrioOp_Shift); /* :ASR: */
		break;
	    }
	  Input_UnGetC (c2);
	  break;
	}
	
      case 'C':
	FINISH_STR ("C:", eOp_Concat, kPrioOp_String); /* :CC: */
	break;
	
      case 'E':
	FINISH_STR ("OR:", eOp_XOr, kPrioOp_AddAndLogical); /* :EOR: */
	break;
	
      case 'L':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'A':
		FINISH_STR ("ND:", eOp_LAnd, kPrioOp_Boolean); /* :LAND: */
		break;
	      case 'E':
		{
		  char c3 = Input_GetC ();
		  switch (c3)
		    {
		      case 'F':
			FINISH_STR ("T:", eOp_Left, kPrioOp_String); /* :LEFT: */
			break;
		      case 'O':
			FINISH_STR ("R:", eOp_LEOr, kPrioOp_Boolean); /* :LEOR: */
			break;
		    }
		  Input_UnGetC (c3);
		  break;
		}
	      case 'O':
		FINISH_STR ("R:", eOp_LOr, kPrioOp_Boolean); /* :LOR: */
		break;
	    }
	  Input_UnGetC (c2);
	  break;
	}
	
      case 'M':
	FINISH_STR ("OD:", eOp_Mod, kPrioOp_Multiplicative); /* :MOD: */
	break;

      case 'O':
	FINISH_STR ("R:", eOp_Or, kPrioOp_AddAndLogical); /* :OR: */
	break;

      case 'R':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'I':
		FINISH_STR ("GHT:", eOp_Right, kPrioOp_String); /* :RIGHT: */
		break;
	      case 'O':
		{
		  char c3 = Input_GetC ();
		  switch (c3)
		    {
		      case 'L':
			FINISH_CHR (eOp_ROL, kPrioOp_Shift); /* :ROL: */
			break;
		      case 'R':
			FINISH_CHR (eOp_ROR, kPrioOp_Shift); /* :ROR: */
			break;
		    }
		  Input_UnGetC (c3);
		  break;
		}
	    }
	  Input_UnGetC (c2);
	  break;
	}
	
      case 'S':
	{
	  char c2 = Input_GetC ();
	  switch (c2)
	    {
	      case 'H':
		{
		  char c3 = Input_GetC ();
		  switch (c3)
		    {
		      case 'L':
			FINISH_CHR (eOp_SHL, kPrioOp_Shift); /* :SHL: */
			break;
		      case 'R':
			FINISH_CHR (eOp_SHR, kPrioOp_Shift); /* :SHR: */
			break;
		    }
		  Input_UnGetC (c3);
		  break;
		}
	    }
	  Input_UnGetC (c2);
	  break;
	}
    }
  Input_UnGetC (c1);

  /* Failed to tokenize.  */
  lex->tag = LexNone;
}


/**
 * Get built-in variable.
 * Built-in variables can be in uppercase, lowercase, or mixed.
 */
static Lex
Lex_GetBuiltinVariable (void)
{
  const char * const inputMark = Input_GetMark ();
  switch (Input_GetC () | 0x20)
    {
      case 'a':
	{
	  if (Input_MatchStringLower ("sasm_version}")) /* {ASASM_VERSION} */
	    return Lex_Int (ASASM_VERSION);

	  if (Input_MatchStringLower ("rchitecture}")) /* {ARCHITECTURE} */
	    {
	      const char *arch = Target_GetArch (true);
	      return Lex_String (arch, strlen (arch), false);
	    }

	  if (Input_MatchStringLower ("reaname}")) /* {AREANAME} */
	    return Lex_String (areaCurrentSymbol->str, areaCurrentSymbol->len, false);
	  break;
	}

      case 'c':
	{
	  /* FIXME: {COMMANDLINE} */
	  if (Input_MatchStringLower ("odesize}")) /* {CODESIZE} */
	    {
	      /* {CODESIZE} is documented as synonym for {CONFIG} but {CONFIG}
	         can be 26 which is really legacy and is a bit odd to have 26
	         as 'code size'.  Hence, {CODESIZE} can only be 16 or 32.  */
	      return Lex_Int (State_GetInstrType () == eInstrType_ARM ? 32 : 16);
	    }

	  if (Input_MatchStringLower ("onfig}")) /* {CONFIG} */
	    return Lex_Int (State_GetInstrType () == eInstrType_ARM ? ((gOptionAPCS & APCS_OPT_32BIT) ? 32 : 26) : 16);

	  if (Input_MatchStringLower ("pu}")) /* {CPU} */
	    {
	      const char *cpu = Target_GetCPU ();
	      return Lex_String (cpu, strlen (cpu), false);
	    }
	  break;
	}

      case 'e':
	{
	  if (Input_MatchStringLower ("ndian}")) /* {ENDIAN} */
	    return Lex_String ("little", sizeof ("little")-1, false);
	  break;
	}

      case 'f':
	{
	  if (Input_MatchStringLower ("alse}")) /* {FALSE} */
	    return Lex_Bool (false);
	  if (Input_MatchStringLower ("pu}")) /* {FPU} */
	    {
	      const char *fpu = Target_GetFPU ();
	      return Lex_String (fpu, strlen (fpu), false);
	    }
	  break;
	}

      case 'i':
	{
	  if (Input_MatchStringLower ("nputfile}")) /* {INPUTFILE} */ 
	    {
	      const char *inputFile = FS_GetCurFileName ();
	      return Lex_String (inputFile, strlen (inputFile), false);
	    }
	  if (Input_MatchStringLower ("nter}")) /* {INTER} */
	    return Lex_Bool ((gOptionAPCS & APCS_OPT_INTERWORK) != 0);
	  break;
	}

      case 'l':
	{
	  if (Input_MatchStringLower ("inenum}")) /* {LINENUM} */
	    return Lex_Int (FS_GetBuiltinVarLineNum ());

	  if (Input_MatchStringLower ("inenumup}")) /* {LINENUMUP} */
	    return Lex_Int (FS_GetBuiltinVarLineNumUp ());

	  if (Input_MatchStringLower ("inenumupper}")) /* {LINENUMUPPER} */
	    return Lex_Int (FS_GetBuiltinVarLineNumUpper ());
	  break;
	}

      case 'o':
	{
	  if (Input_MatchStringLower ("pt}")) /* {OPT} */
	    return Lex_Int (gOpt_DirectiveValue);
	  break;
	}

      case 'p':
	{
	  /* FIXME: {PCSTOREOFFSET} */
	  if (Input_MatchStringLower ("c}")) /* {PC} */
	    {
	      const Lex result = { .tag = LexPosition };
	      return result;
	    }
	  break;
	}

      case 'r':
	{
	  if (Input_MatchStringLower ("opi}")) /* {ROPI} */
	    return Lex_Bool ((gOptionAPCS & APCS_OPT_ROPI) != 0);

	  if (Input_MatchStringLower ("eentrant}") /* {REENTRANT} */
	      || Input_MatchKeywordLower ("wpi}")) /* {RWPI} */
	    return Lex_Bool ((gOptionAPCS & APCS_OPT_REENTRANT) != 0);
	  break;
	}

      case 't':
	{
	  if (Input_MatchStringLower ("arget_"))
	    {
	      if (Input_MatchStringLower ("arch_"))
		{
		  if (Input_MatchStringLower ("arm}")) /* {TARGET_ARCH_ARM} */
		    {
		      /* Number ARM architecture, or 0 if Thumb-only ISA.  */
		      return Lex_Int (Target_GetARMISAVersion ());
		    }
		  if (Input_MatchStringLower ("thumb}")) /* {TARGET_ARCH_THUMB} */
		    {
		      /* Number Thumb architecture, or 0 if ARM-only ISA.  */
		      return Lex_Int (Target_GetThumbISAVersion ());
		    }

		  /* Table mapping the lowercase {TARGET_ARCH_<arch>} suffix
		     to the normalized <arch> version.  */
		  static const struct
		    {
		      const char *arch_in;
		      const char *arch_norm;
		    } oArchs[] =
		    {
		      { "1}", "1" },
      		      { "2}", "2" },
      		      { "2a}", "2a" },
      		      { "3}", "3" },
      		      { "3g}", "3G" },
      		      { "3m}", "3M" },
      		      { "4xm}", "4xM" },
      		      { "4}", "4" },
      		      { "4txm}", "4TxM" },
      		      { "4t}", "4T" },
      		      { "5xm}", "5xM" },
      		      { "5}", "5" },
      		      { "5txm}", "5TxM" },
      		      { "5t}", "5T" },
      		      { "5texp}", "5TExP" },
      		      { "5te}", "5TE" },
      		      { "5tej}", "5TEJ" },
      		      { "6}", "6" },
      		      { "6k}", "6K" },
      		      { "6t2}", "6T2" },
      		      { "6z}", "6Z" },
      		      { "6_m}", "6-M" },
      		      { "6s_m}", "6S-M" },
      		      { "7}", "7" },
      		      { "7_a}", "7-A" },
      		      { "7_r}", "7-R" },
      		      { "7_m}", "7-M" },
      		      { "7e_m}", "7E-M" },
		      { "8}", "8" },
		      { "8_a}", "8-A" },
		    };
		  for (size_t i = 0; i != sizeof (oArchs)/sizeof (oArchs[0]); ++i)
		    {
		      if (Input_MatchStringLower (oArchs[i].arch_in)) /* {TARGET_ARCH_<architecture>} */
			return Lex_Bool (!strcmp (Target_GetArch (false), oArchs[i].arch_norm));
		    }
		}
	      else if (Input_MatchStringLower ("feature_"))
		{
		  if (Input_MatchStringLower ("clz}")) /* {TARGET_FEATURE_CLZ} */
		    {
		      /* True when CLZ is available in Thumb *or* ARM.
		          So needs at least v5 architecture (ARM) or Thumb v4 (Thumb).  */
		      return Lex_Bool (Target_GetARMISAVersion () >= 5
				       || Target_GetThumbISAVersion () >= 4);
		    }
		  if (Input_MatchStringLower ("divide}")) /* {TARGET_FEATURE_DIVIDE} */
		    {
		      /* True when SDIV/UDIV is available in Thumb *or* ARM.  */
		      return Lex_Bool ((Target_GetCPUFeatures () & kCPUExt_Div) != 0);
		    }
		  if (Input_MatchStringLower ("doubleword}")) /* {TARGET_FEATURE_DOUBLEWORD} */
		    {
		      /* True when LDRD/STRD is available (P extension).
			 So needs at least v5E architecture but not for ARMv6-M.  */
		      uint64_t features = Target_GetCPUFeatures ();
		      return Lex_Bool ((features & kCPUExt_v5E) != 0
				       && ((features & (kCPUExt_v6M | kCPUExt_v7M)) != kCPUExt_v6M));
		    }
		  if (Input_MatchStringLower ("dspmul}")) /* {TARGET_FEATURE_DSPMUL} */
		    {
		      /* True when DSP & saturated math is available (E extension).  */
		      return Lex_Bool ((Target_GetCPUFeatures () & kCPUExt_v5ExP) != 0);
		    }
		  if (Input_MatchStringLower ("extension_register_count}")) /* {TARGET_FEATURE_EXTENSION_REGISTER_COUNT} */
		    {		      
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Int ((features & (kFPUExt_VFPv1 | kFPUExt_VFPv2 | kFPUExt_VFPv3)) != 0 ? ((features & kFPUExt_D16D32) ? 32 : 16) : 0);
		    }
		  if (Input_MatchStringLower ("multiply}")) /* {TARGET_FEATURE_MULTIPLY} */
		    {
		      /* True when long multiply instructions are available in
			 Thumb *or* ARM (M extension).  */
		      uint64_t features = Target_GetCPUFeatures ();
		      return Lex_Bool ((features & (kCPUExt_v3M | kCPUExt_v7M)) != 0
				       || features == kArchExt_v7);
		    }
		  if (Input_MatchStringLower ("multiprocessing}")) /* {TARGET_FEATURE_MULTIPROCESSING} */
		    {
		      /* True when multiprocessing extensions are supported.  */
		      return Lex_Bool ((Target_GetCPUFeatures () & kCPUExt_MP) != 0);
		    }
		  if (Input_MatchStringLower ("neon}")) /* {TARGET_FEATURE_NEON} */
		    {
		      /* True when there is NEON support.  */
		      return Lex_Bool ((Target_GetFPUFeatures () & kFPUExt_NEONv1) != 0);
		    }
		  if (Input_MatchStringLower ("neon_fp16}")) /* {TARGET_FEATURE_NEON_FP16} */
		    {
		      /* True when there is half-precision NEON support.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_NEONv1 | kFPUExt_FP16)) == (kFPUExt_NEONv1 | kFPUExt_FP16));
		    }
		  if (Input_MatchStringLower ("neon_fp32}")) /* {TARGET_FEATURE_NEON_FP32} */
		    {
		      /* True when there is single-precision NEON support.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & kFPUExt_NEONv1) != 0
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0);
		    }
		  if (Input_MatchStringLower ("neon_integer}")) /* {TARGET_FEATURE_NEON_INTEGER} */
		    {
		      /* True when there is NEON integer-only support (i.e. NEON but no VFP at all).  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & kFPUExt_NEONv1) != 0
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == 0);
		    }
		  if (Input_MatchStringLower ("unaligned}")) /* {TARGET_FEATURE_UNALIGNED} */
		    {
		      /* True when unaligned access is supported.
		         From ARMv6 onwards but not for ARMv6-M.  */
		      uint64_t features = Target_GetCPUFeatures ();
		      return Lex_Bool ((features & kCPUExt_v6) != 0
				       && (features & (kCPUExt_v6M | kCPUExt_v7M)) != kCPUExt_v6M);
		    }
		}
	      else if (Input_MatchStringLower ("fpu_"))
		{
		  if (Input_MatchStringLower ("fpa}")) /* {TARGET_FPU_FPA} */
		    {
		      /* True when FPA FPU is selected.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == 0
				       && (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) != 0);
		    }
		  if (Input_MatchStringLower ("softfpa}")) /* {TARGET_FPU_SOFTFPA} */
		    {
		      /* True when FPA endian + float-abi == soft.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2 | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == kFPUExt_SoftFPA);  
		    }
		  if (Input_MatchStringLower ("softfpa_fpa}")) /* {TARGET_FPU_SOFTFPA_FPA} */
		    {
		      /* True when FPA endian + float-abi == softfp (using FPA).  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == kFPUExt_SoftFPA
				       && (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) != 0);
		    }
		  if (Input_MatchStringLower ("softfpa_vfp}")) /* {TARGET_FPU_SOFTFPA_VFP} */
		    {
		      /* True when FPA endian + float-abi == softfp (using VFP).  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2)) == kFPUExt_SoftFPA
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0);  
		    }
		  if (Input_MatchStringLower ("softvfp}")) /* {TARGET_FPU_SOFTVFP} */
		    {
		      /* True when VFP endian + float-abi == soft.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2 | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == (kFPUExt_NoEndianMismatch | kFPUExt_SoftVFP));  
		    }
		  if (Input_MatchStringLower ("softvfp_fpa}")) /* {TARGET_FPU_SOFTVFP_FPA} */
		    {
		      /* True when VFP endian + float-abi == softfp (using FPA).  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == (kFPUExt_NoEndianMismatch | kFPUExt_SoftVFP)
				       && (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) != 0);  
		    }
		  if (Input_MatchStringLower ("softvfp_vfp}")) /* {TARGET_FPU_SOFTVFP_VFP} */
		    {
		      /* True when VFP endian + float-abi == softfp (using VFP).  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2)) == (kFPUExt_NoEndianMismatch | kFPUExt_SoftVFP)
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0);  
		    }
		  if (Input_MatchStringLower ("vfp}")) /* {TARGET_FPU_VFP} */
		    {
		      /* True when VFP FPU is selected.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0);
		    }
		  if (Input_MatchStringLower ("vfpv1}")) /* {TARGET_FPU_VFPV1} */
		    {
		      /* True when VFPv1 is selected. AsAsm extension.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_VFP_FMA)) == kFPUExt_VFPv1xD);
		    }
		  if (Input_MatchStringLower ("vfpv2}")) /* {TARGET_FPU_VFPV2} */
		    {
		      /* True when VFPv2 is selected.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_VFP_FMA)) == (kFPUExt_VFPv1xD | kFPUExt_VFPv2));
		    }
		  if (Input_MatchStringLower ("vfpv3}")) /* {TARGET_FPU_VFPV3} */
		    {
		      /* True when VFPv3 is selected.  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA)) != (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA));
		    }
		  if (Input_MatchStringLower ("vfpv4}")) /* {TARGET_FPU_VFPV4} */
		    {
		      /* True when VFPv4 is selected (i.e. VFPv3 + FP16 + VFP_FMA).  */
		      unsigned features = Target_GetFPUFeatures ();
		      return Lex_Bool ((features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
				       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA)) == (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA));
		    }
		  /* FIXME: ARMv8 VFP/NEON/Crypto missing ? */
		}
	      else if (Input_MatchStringLower ("profile_"))
		{
		  if (Input_MatchStringLower ("a}")) /* {TARGET_PROFILE_A} */
		    {
		      /* True for ARMv7-A and ARMv8-A.  */
		      uint64_t features = Target_GetCPUFeatures ();
		      return Lex_Bool ((features & (kCPUExt_v7A | kCPUExt_v8A)) != 0);
		    }
		  if (Input_MatchStringLower ("m}")) /* {TARGET_PROFILE_M} */
		    {
		      /* True for ARMv6-M or ARMv7-M.  */
		      uint64_t features = Target_GetCPUFeatures ();
		      return Lex_Bool ((features & (kCPUExt_v6M | kCPUExt_v7M)) != 0);
		    }
		  if (Input_MatchStringLower ("r}")) /* {TARGET_PROFILE_R} */
		    {
		      /* True for ARMv7-R.  */
		      return Lex_Bool ((Target_GetCPUFeatures () & kCPUExt_v7R) != 0);
		    }
		}
	    }
	  else if (Input_MatchStringLower ("rue}")) /* {TRUE} */
	    return Lex_Bool (true);
	  break;
	}

      case 'u':
	{
	  if (Input_MatchStringLower ("al}")) /* {UAL} */
	    {
	      /* True when only UAL syntax is accepted.  */
	      return Lex_Bool (State_GetSyntax() == eSyntax_UALOnly);
	    }
	  break;
	}
      case 'v':
	{
	  if (Input_MatchStringLower ("ar}")) /* {VAR} */
	    {
	      const Lex result = { .tag = LexStorage };
	      return result;
	    }
	  break;
	}
    }

  /* Try to find the end of the built-in variable name.  */
  const char *lineRest = Input_Rest ();
  while (*lineRest != '\0' && *lineRest != '\n' && *lineRest != '}')
    ++lineRest;
  if (*lineRest == '}')
    Error (ErrorError, "Unknown built-in variable {%.*s",
	   (int)(lineRest + 1 - inputMark), inputMark);
  else
    Error (ErrorError, "Missing closing bracket");

  return Lex_None ();
}


/**
 * Tries to parse an integer (LexInt or LexInt64) or float (LexFloat) literal.
 * Any base indication (like "0x", "&", "4_", etc.) has already been parsed.
 *
 *   <mantissa> [ "E" | "e" | "P" | "p" [ "+" | "-" | "" <exponent> ] ]
 */
static Lex
Lex_GetIntOrFloat (unsigned base)
{
  assert ((base >= 2 && base <= 10) || base == 16);

  const char *mark = Input_GetMark ();
  bool didOverflow = false;

  uint64_t res;
  unsigned char c;
  bool atLeastOneDigit = false;
  for (res = 0; isxdigit (c = Input_LookLower ()); Input_Skip ())
    {
      unsigned val = c - (c >= 'a' ? 'a' - 10 : '0');

      if (val >= base)
	break;

      atLeastOneDigit = true;
      
      uint64_t resNext = res * base + val;
      if (resNext < res)
	didOverflow = true;
      res = resNext;
    }

  if (c == '.' || c == 'E' || c == 'e' || c == 'P' || c == 'p')
    {
      if (base != 10 && base != 16)
	{
	  Error (ErrorError, "No base 10 or 16 floating-point literal is not supported"); /* FIXME */
	  return Lex_None (); 
	}
      /* Find end of floating-point literal.  */
      /* Skip digits after the dot in the mantissa.  */
      if (c == '.')
	{
	  Input_Skip ();
	  while (base == 10 ? isdigit ((unsigned char)Input_Look ()) : isxdigit ((unsigned char)Input_Look ()))
	    Input_Skip ();
	  c = Input_Look ();
	}
      if (c == 'E' || c == 'e' || c == 'P' || c == 'p')
	{
	  Input_Skip ();
	  /* Skip optional exponent sign character.  */
	  if (!Input_Match ('-', false))
	    Input_Match ('+', false);
	  /* Skip exponent.  */
	  while (isdigit ((unsigned char)Input_Look ()))
	    Input_Skip ();
	}
      const char *markEnd = Input_GetMark ();
      /* Floating-point literal goes from 'mark' - 'markEnd'.  */
      size_t sizeNeeded = markEnd - mark + 1;
      if (base != 10)
	sizeNeeded += sizeof ("0x")-1;
      char *fltLiteral = malloc (sizeNeeded);
      if (fltLiteral == NULL)
	Error_OutOfMem ();
      sprintf (fltLiteral, "%s%.*s", base == 10 ? "" : "0x", (int)(markEnd - mark), mark);
      char *markEnd2;
      double d = strtod (fltLiteral, &markEnd2);
      free (fltLiteral);
      if (markEnd2 != fltLiteral + sizeNeeded - 1)
	{
	  Error (ErrorError, "Failed to read floating point value");
	  return Lex_None ();
	}

      return Lex_Float (d);
    }

  /* Integer.
     We must have read at least one digit (e.g. "&XX" is not valid).  */
  if (!atLeastOneDigit)
    return Lex_None ();

  if (res <= UINT32_MAX)
    {
      const Lex lexInt32 =
	{
	  .tag = LexInt,
	  .Data.Int = { .value = (int)res }
	};
      return lexInt32;
    }
  if (didOverflow)
    Error (ErrorWarning, "64-bit integer overflow");

  const Lex lexInt64 =
    {
      .tag = LexInt64,
      .Data.Int64 = { .value = res }
    };
  return lexInt64;
}


/**
 * Parse floating point literal starting with "0f_" + followed by exactly
 * 8 hex digits.
 * The "0f_" part is already parsed.
 */
static Lex
Lex_GetFloatFloatingPointLiteral (void)
{
  uint32_t fltAsInt = 0;
  for (unsigned i = 0; i != 8; Input_Skip (), ++i)
    {
      unsigned char c = Input_LookLower ();
      if (!isxdigit (c))
	{
	  Error (ErrorError, "Float floating point literal needs exactly 8 hex digits");
	  return Lex_None ();
	}
      fltAsInt = 16*fltAsInt + c - ((c >= 'a') ? 'a' - 10 : '0');
    }
  const union arm_float armflt = { .i = fltAsInt };
  const union ieee754_float flt =
    {
      .ieee =
	{
	  .mantissa = armflt.flt.mantissa,
	  .exponent = armflt.flt.exponent,
	  .negative = armflt.flt.negative
	}
    };

  return Lex_Float (flt.f);
}


/**
 * Parse floating point literal starting with "0d_" + followed by exactly
 * 16 hex digits.
 * The "0d_" part is already parsed.
 */
static Lex
Lex_GetDoubleFloatingPointLiteral (void)
{
  uint64_t dblAsInt = 0;
  for (unsigned i = 0; i != 16; Input_Skip (), ++i)
    {
      unsigned char c = Input_LookLower ();
      if (!isxdigit (c))
	{
	  Error (ErrorError, "Double floating point literal needs exactly 16 hex digits");
	  return Lex_None ();
	}
      dblAsInt = 16*dblAsInt + c - ((c >= 'a') ? 'a' - 10 : '0');
    }
  double d;
  if ((areaCurrentSymbol->attr.area->type & AREA_VFP) != 0)
    {
      const union arm_double_vfp armdbl_vfp = { .i = dblAsInt };
      const union ieee754_double dbl =
	{
	  .ieee =
	    {
	      .mantissa0 = armdbl_vfp.dbl.mantissa0,
	      .exponent = armdbl_vfp.dbl.exponent,
	      .negative = armdbl_vfp.dbl.negative,
	      .mantissa1 = armdbl_vfp.dbl.mantissa1
	    }
	};
      d = dbl.d;
    }
  else
    {
      const union arm_double_fpa armdbl_fpa = { .i = dblAsInt };
      const union ieee754_double dbl =
	{
	  .ieee =
	    {
	      .mantissa0 = armdbl_fpa.dbl.mantissa0,
	      .exponent = armdbl_fpa.dbl.exponent,
	      .negative = armdbl_fpa.dbl.negative,
	      .mantissa1 = armdbl_fpa.dbl.mantissa1
	    }
	};
      d = dbl.d;
    }

  return Lex_Float (d);
}


Lex
Lex_GetPrim (void)
{
  Input_SkipWS ();

  Lex result;
  char c = Input_GetC ();
  switch (c)
    {
      case '+':
	{
	  result.tag = LexOperator;
	  result.Data.Operator.op = eOp_Pos; /* +XYZ */
	  result.Data.Operator.pri = kPrioOp_Unary;
	  break;
	}

      case '-':
	{
	  result.tag = LexOperator;
	  result.Data.Operator.op = eOp_Neg; /* -XYZ */
	  result.Data.Operator.pri = kPrioOp_Unary;
	  break;
	}

      case '!':
	{
	  result.tag = LexOperator;
	  result.Data.Operator.op = eOp_LNot; /* !XYZ */
	  result.Data.Operator.pri = kPrioOp_Unary;
	  break;
	}

      case '~':
	{
	  result.tag = LexOperator;
	  result.Data.Operator.op = eOp_Not; /* ~XYZ */
	  result.Data.Operator.pri = kPrioOp_Unary;
	  break;
	}

      case '?':
	{
	  result.tag = LexOperator;
	  result.Data.Operator.op = eOp_Size; /* ?<label> */
	  result.Data.Operator.pri = kPrioOp_Unary;
	  break;
	}

      case '(':
      case ')':
	{
	  result.tag = LexDelim;
	  result.Data.Delim.delim = c;
	  break;
	}

      case ':':
	{
	  Lex_GetUnaryOp (&result);
	  if (result.tag == LexNone)
	    Input_UnGetC (':');
	  break;
	}

      case '&':
	{
	  result = Lex_GetIntOrFloat (16);
	  break;
	}

      case '0':
	{
	  /* Floating point literal 0f_... or 0d_... ?
	     Or hexademical integer or floating-point literal ? */
	  char c0 = Input_LookN (0);
	  if (c0 == 'f' && Input_LookN (1) == '_')
	    {
	      Input_SkipN (sizeof ("f_")-1); /* Skip "f_" */
	      return Lex_GetFloatFloatingPointLiteral ();
	    }

	  if (c0 == 'd' && Input_LookN (1) == '_')
	    {
	      Input_SkipN (sizeof ("d_")-1); /* Skip "d_" */
	      return Lex_GetDoubleFloatingPointLiteral ();
	    }

	  if (c0 == 'x' || c0 == 'X')
	    {
	      Input_Skip ();
	      return Lex_GetIntOrFloat (16);
	    }
	  /* Fall through.  */
	}
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
	{
	  unsigned base;
	  if (Input_Match ('_', false))
	    {
	      /* base is specified.  */
	      base = c - '0';
	      if (base < 2 || base > 9)
		{
		  Error (ErrorError, "Illegal base %d", base);
		  Input_UnGetC (c);
		  result = Lex_None ();
		  break;
		}
	    }
	  else
	    {
	      base = 10;
	      Input_UnGetC (c);
	    }
	  result = Lex_GetIntOrFloat (base);
	  break;
	}

      case '\'':
	{
	  uint32_t intResult = 0;
	  unsigned char ci;
	  if (Input_Look () == '\'')
	    Error (ErrorError, "Empty character constant");
	  else
	    for (unsigned i = 0; (ci = Input_GetC ()) != '\''; ++i)
	      {
		if (ci == '\\')
		  ci = Input_GetC (); /* FIXME: not 100% ok */
		if (ci == '\0')
		  {
		    Error (ErrorError, "Constant specification continues over newline");
		    break;
		  }
		if (i == 1)
		  Error (ErrorWarning, "Non-standard multi-character character constant");
		else if (i == 4)
		  {
		    Error (ErrorError, "Illegal constant specification");
		    break;
		  }
		intResult <<= 8;
		intResult |= ci;
	      }
	  result.tag = LexInt;
	  result.Data.Int.value = intResult;
	  break;
	}

      case '"':
	{
	  size_t len;
	  const char *str = Input_GetString (&len);
	  result = Lex_String (str, len, true);
	  break;
	}

      case '.':
	{
	  /* Do we have the position mark '.' or start of a floating point
	     number ? */
	  if (isdigit ((unsigned char)Input_Look ()))
	    {
	      /* Looks like a floating point number.  */
	      Input_UnGetC (c);
	      result = Lex_GetIntOrFloat (10);
	    }
	  else
	    result.tag = LexPosition;
	  break;
	}

      case '@':
	result.tag = LexStorage;
	break;

      case '%':
	{
	  /* Local label reference.  */
	  LocalLabel_eDir dir;
	  switch (Input_LookLower ())
	    {
	      case 'f':
		/* Forward looking.  */
		Input_Skip ();
		dir = eForward;
		break;
  
	      case 'b':
		/* Backward looking.  */
		Input_Skip ();
		dir = eBackward;
		break;
  
	      default:
		/* Search backwards first, then forward.  */
		dir = eBackwardThenForward;
		break;
	    }
	  LocalLabel_eLevel level;
	  switch (Input_LookLower ())
	    {
	      case 't':
		/* Only at this macro level.  */
		Input_Skip ();
		level = eThisLevelOnly;
		break;
  
	      case 'a':
		/* All macro levels.  */
		Input_Skip ();
		level = eAllLevels;
		break;
  
	      default:
		/* At this macro level and all upper levels.  */
		level = eThisLevelAndHigher;
		break;
	    }
	  return Lex_MakeReferringLocalLabel (dir, level);
	}

      case '{':
	return Lex_GetBuiltinVariable ();

      default:
	/* Try to read a symbol.  */
	Input_UnGetC (c);
	if ((result.Data.Id.str = Input_Symbol (&result.Data.Id.len)) != NULL)
	  result.tag = LexId;
	else
	  result.tag = LexNone;
	break;
    }

  return result;
}

Lex
Lex_GetBinop (void)
{
  Input_SkipWS ();
  Lex result;
  int c;
  switch (c = Input_GetC ())
    {
      case '*':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_Mul; /* * */
	result.Data.Operator.pri = kPrioOp_Multiplicative;
	break;

      case '/':
	result.tag = LexOperator;
	if (Input_Match ('=', false))
	  {
	    result.Data.Operator.op = eOp_NE; /* /= */
	    result.Data.Operator.pri = kPrioOp_Relational;
	  }
	else
	  {
	    result.Data.Operator.op = eOp_Div; /* / */
	    result.Data.Operator.pri = kPrioOp_Multiplicative;
	  }
	break;

      case '%':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_Mod; /* % MOD */
	result.Data.Operator.pri = kPrioOp_Multiplicative;
	break;

      case '+':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_Add; /* + */
	result.Data.Operator.pri = kPrioOp_AddAndLogical;
	break;

      case '-':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_Sub; /* - */
	result.Data.Operator.pri = kPrioOp_AddAndLogical;
	break;

      case '>':
	result.tag = LexOperator;
	switch (Input_Look ())
	  {
	    case '>':
	      Input_Skip ();
	      result.Data.Operator.pri = kPrioOp_Shift;
	      if (Input_Match ('>', false))
		result.Data.Operator.op = eOp_ASR; /* >>> */
	      else
		result.Data.Operator.op = eOp_SHR; /* >> */
	      break;

	    case '=':
	      Input_Skip ();
	      result.Data.Operator.op = eOp_GE;
	      result.Data.Operator.pri = kPrioOp_Relational; /* >= */
	      break;
  
	    default:
	      result.Data.Operator.op = eOp_GT;
	      result.Data.Operator.pri = kPrioOp_Relational; /* > */
	      break;
	  }
	break;

      case '<':
	result.tag = LexOperator;
	switch (Input_Look ())
	  {
	    case '<':
	      Input_Skip ();
	      result.Data.Operator.op = eOp_SHL; /* << */
	      result.Data.Operator.pri = kPrioOp_Shift;
	      break;
    
	    case '=':
	      Input_Skip ();
	      result.Data.Operator.op = eOp_LE; /* <= */
	      result.Data.Operator.pri = kPrioOp_Relational;
	      break;
    
	    case '>':
	      Input_Skip ();
	      result.Data.Operator.op = eOp_NE; /* <> */
	      result.Data.Operator.pri = kPrioOp_Relational;
	      break;
    
	    default:
	      result.Data.Operator.op = eOp_LT; /* < */
	      result.Data.Operator.pri = kPrioOp_Relational;
	      break;
	  }
	break;

      case '=':
	Input_Match ('=', false); /* Deals with '==' */
	result.tag = LexOperator;
	result.Data.Operator.pri = kPrioOp_Relational;
	result.Data.Operator.op = eOp_EQ; /* = == */
	break;

      case '!':
	if (Input_Match ('=', false))
	  {
	    result.tag = LexOperator;
	    result.Data.Operator.pri = kPrioOp_Relational;
	    result.Data.Operator.op = eOp_NE; /* != */
	  }
	else
	  {
	    Input_UnGetC (c);
	    result.tag = LexNone;
	  }
	break;

      case '|':
	result.tag = LexOperator;
	if (Input_Match ('|', false))
	  {
	    result.Data.Operator.pri = kPrioOp_Boolean;
	    result.Data.Operator.op = eOp_LOr; /* || */
	  }
	else
	  {
	    result.Data.Operator.pri = kPrioOp_AddAndLogical;
	    result.Data.Operator.op = eOp_Or; /* | */
	  }
	break;

      case '&':
	result.tag = LexOperator;
	if (Input_Match ('&', false))
	  {
	    result.Data.Operator.pri = kPrioOp_Boolean;
	    result.Data.Operator.op = eOp_LAnd; /* && */
	  }
	else
	  {
	    result.Data.Operator.pri = kPrioOp_AddAndLogical;
	    result.Data.Operator.op = eOp_And; /* & */
	  }
	break;

      case ':':
	Lex_GetBinaryOp (&result); /* :XYZ: */
	if (result.tag == LexNone)
	  Input_UnGetC (':');
	break;

      case '^':
	{
	  /* Check if there is anything meaningful following.  If not, it might
	     then be the ^ in a LDMIA register list.  */
	  const char *mark = Input_GetMark ();
	  Input_SkipWS ();
	  bool somethingElse = !Input_IsEolOrCommentStart ();
	  Input_RollBackToMark (mark);
	  if (somethingElse)
	    {
	      result.tag = LexOperator;
	      result.Data.Operator.pri = kPrioOp_AddAndLogical;
	      result.Data.Operator.op = eOp_XOr; /* ^ */
	      break;
	    }
	  /* Fall through.  */
	}

      default:
	Input_UnGetC (c);
	result.tag = LexNone;
	break;
    }

  return result;
}

#ifdef DEBUG
void
Lex_Print (const Lex *lex)
{
  if (lex == NULL)
    {
      printf ("<NULL> ");
      return;
    }
  switch (lex->tag)
    {
      case LexId:
	printf ("Id <%.*s> ", (int)lex->Data.Id.len, lex->Data.Id.str);
	break;
      case LexLocalLabel:
	printf ("LocalLabel <%.*s> ", (int)lex->Data.LocalLabel.len, lex->Data.LocalLabel.str);
	break;
      case LexString:
	printf ("Str <%.*s> (%s)", (int)lex->Data.String.len, lex->Data.String.str, lex->Data.String.owns ? "owns" : "NO ownership");
	break;
      case LexInt:
	printf ("Int <%d = 0x%x> ", lex->Data.Int.value, lex->Data.Int.value);
	break;
      case LexInt64:
	printf ("Int64 <%ld = 0x%lx> ", (int64_t)lex->Data.Int64.value, lex->Data.Int64.value);
	break;
      case LexFloat:
	printf ("Flt <%g> ", lex->Data.Float.value);
	break;
      case LexOperator:
	printf ("Op %s (pri %d) ", Lex_OperatorAsStr (lex->Data.Operator.op), lex->Data.Operator.pri);
	break;
      case LexPosition:
	printf ("Pos ");
	break;
      case LexStorage:
	printf ("Stor ");
	break;
      case LexDelim:
	printf ("Delim <%d> ", lex->Data.Delim.delim);
	break;
      case LexBool:
	printf("bool <%s> ", lex->Data.Bool.value ? "true" : "false");
	break;
      case LexNone:
	printf ("None ");
	break;
      default:
	printf ("Unknown lex tag 0x%x ", lex->tag);
	break;
    }
}

const char *
Lex_OperatorAsStr (Operator_e op)
{
  static const char * const opStr[] =
    {
      ":FLOAD:",	/* eOp_FLoad */
      ":FEXEC:",	/* eOp_FExec */
      ":FSIZE:",	/* eOp_FSize */
      ":FATTR:",	/* eOp_FAttr */
      ":LNOT:",		/* eOp_LNot */
      ":NOT:",		/* eOp_Not */
      "-",		/* eOp_Neg */
      "+",		/* eOp_Pos */
      ":BASE:",		/* eOp_Base */
      ":INDEX:",	/* eOp_Index */
      ":LEN:",		/* eOp_Len */
      ":STR:",		/* eOp_Str */
      ":CHR:",		/* eOp_Chr */
      ":SIZE:",		/* eOp_Size */

      ":LOWERCASE:",	/* eOp_LowerCase */
      ":UPPERCASE:",	/* eOp_UpperCase */
      ":REVERSE_CC:",	/* eOp_RevCC */

      ":CC_ENCODING:",	/* eOp_CCEnc */
      ":RCONST:",	/* eOp_RConst */

      ":MUL:",		/* eOp_Mul */
      ":DIV:",		/* eOp_Div */
      ":MOD:",		/* eOp_Mod */

      ":LEFT:",		/* eOp_Left */
      ":RIGHT:",	/* eOp_Right */
      ":CC:",		/* eOp_Concat */

      ":ASR:",		/* eOp_ASR */
      ":SHR:",		/* eOp_SHR */
      ":SHL:",		/* eOp_SHL */
      ":ROR:",		/* eOp_ROR */
      ":ROL:",		/* eOp_ROL */
      
      ":ADD:",		/* eOp_Add */
      ":SUB:",		/* eOp_Sub */
      ":AND:",		/* eOp_And */
      ":OR:",		/* eOp_Or */
      ":XOR:",		/* eOp_XOr */

      ":LE:",		/* eOp_LE */
      ":GE:",		/* eOp_GE */
      ":LT:",		/* eOp_LT */
      ":GT:",		/* eOp_GT */
      ":EQ:",		/* eOp_EQ */
      ":NE:",		/* eOp_NE */

      ":LAND:",		/* eOp_LAnd */
      ":LOR:",		/* eOp_LOr */
      ":LEOR:",		/* eOp_LEOr */
    };
  return op >= sizeof (opStr) / sizeof (opStr[0]) ? ":???:" : opStr[op];
}

#endif
