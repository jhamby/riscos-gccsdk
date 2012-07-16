/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
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
 * lex.c
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

static Lex nextbinop;
static bool nextbinopvalid = false;

/**
 * A simple and fast generic string hasher based on Peter K. Pearson's
 * article in CACM 33-6, pp. 677.
 * \param s string to hash
 * \param maxn maximum number of chars to consider
 */
static unsigned int
lexHashStr (const char *s, size_t maxn)
{
  static const unsigned char hashtab[256] =
  {
    1, 87, 49, 12, 176, 178, 102, 166, 121, 193, 6, 84, 249, 230, 44, 163,
    14, 197, 213, 181, 161, 85, 218, 80, 64, 239, 24, 226, 236, 142, 38, 200,
    110, 177, 104, 103, 141, 253, 255, 50, 77, 101, 81, 18, 45, 96, 31, 222,
    25, 107, 190, 70, 86, 237, 240, 34, 72, 242, 20, 214, 244, 227, 149, 235,
    97, 234, 57, 22, 60, 250, 82, 175, 208, 5, 127, 199, 111, 62, 135, 248,
    174, 169, 211, 58, 66, 154, 106, 195, 245, 171, 17, 187, 182, 179, 0, 243,
    132, 56, 148, 75, 128, 133, 158, 100, 130, 126, 91, 13, 153, 246, 216, 219,
    119, 68, 223, 78, 83, 88, 201, 99, 122, 11, 92, 32, 136, 114, 52, 10,
    138, 30, 48, 183, 156, 35, 61, 26, 143, 74, 251, 94, 129, 162, 63, 152,
    170, 7, 115, 167, 241, 206, 3, 150, 55, 59, 151, 220, 90, 53, 23, 131,
    125, 173, 15, 238, 79, 95, 89, 16, 105, 137, 225, 224, 217, 160, 37, 123,
    118, 73, 2, 157, 46, 116, 9, 145, 134, 228, 207, 212, 202, 215, 69, 229,
    27, 188, 67, 124, 168, 252, 42, 4, 29, 108, 21, 247, 19, 205, 39, 203,
    233, 40, 186, 147, 198, 192, 155, 33, 164, 191, 98, 204, 165, 180, 117, 76,
    140, 36, 210, 172, 41, 54, 159, 8, 185, 232, 113, 196, 231, 47, 146, 120,
    51, 65, 28, 144, 254, 221, 93, 189, 194, 139, 112, 43, 71, 109, 184, 209,
  };

  const unsigned char *p;
  unsigned char h = 0;
  size_t i;
  for (i = 0, p = (const unsigned char *)s; *p && i < maxn; i++, p++)
    h = hashtab[h ^ *p];

  unsigned int rh = h;

  if (SYMBOL_TABLESIZE > 256 && *s)
    {
      for (i = 1, p = (const unsigned char *)s, h = (*p++ + 1) & 0xff;
	   *p && i < maxn; i++, p++)
	h = hashtab[h ^ *p];

      rh <<= 8;
      rh |= h;
    }
  return rh % SYMBOL_TABLESIZE;
}


/**
 * Parses integer and returns this as LexInt or LexInt64 Lex object.
 * \param base When 16, use base 16.  Any other base value is considered
 * as default base value and can be overruled by "<base>_" (with <base> a
 * digit from 2 to 9 (incl)) or "0x"/"0X" prefix.
 * \param didOverflow Indicates whether an 64-bit integer overflow happened.
 */
static Lex
Lex_GetInt (int base, bool *didOverflow)
{
  *didOverflow = false;

  /* Determine base (when not forced to 16).  */
  if (base != 16)
    {
      char c1, c2;
      if ((c1 = inputLookN (0)) == '0' && ((c2 = inputLookN (1)) == 'x' || c2 == 'X'))
	{
	  inputSkipN (2);
	  base = 16; /* Overrule default base value.  */
	}
      else if (inputLookN (1) == '_')
	{
	  inputSkipN (2);
	  base = c1 - '0'; /* Overrule default base value.  */
	  if (base < 2 || base > 9)
	    error (ErrorError, "Illegal base %d", base);
	}
    }

  uint64_t res;
  char c;
  bool atLeastOneDigit = false;
  for (res = 0; isxdigit (c = inputLookLower ()); inputSkip ())
    {
      c -= (c >= 'a') ? 'a' - 10 : '0';

      if (c >= base)
	break;

      atLeastOneDigit = true;
      
      uint64_t resNext = res * base + c;
      if (resNext < res)
	*didOverflow = true;
      res = resNext;
    }

  if (!atLeastOneDigit)
    {
      const Lex badInt = { .tag = LexNone };
      return badInt;
    }
  if (res <= UINT32_MAX)
    {
      const Lex lexInt32 =
	{
	  .tag = LexInt,
	  .Data.Int = { .value = (int)res }
	};
      return lexInt32;
    }
  const Lex lexInt64 =
    {
      .tag = LexInt64,
      .Data.Int64 = { .value = res }
    };
  return lexInt64;
}


Lex
lexGetIdNoError (void)
{
  nextbinopvalid = false;

  skipblanks ();

  Lex result;
  if ((result.Data.Id.str = Input_Symbol (&result.Data.Id.len)) != NULL)
    {
      result.tag = LexId;
      result.Data.Id.hash = lexHashStr (result.Data.Id.str, result.Data.Id.len);
    }
  else
    result.tag = LexNone;

  return result;
}


Lex
lexGetId (void)
{
  Lex result = lexGetIdNoError ();
  if (result.tag != LexId)
    error (ErrorError, "Missing or wrong identifier");
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
	error (ErrorError, "Local label can not have a routine name %.*s here", (int)routLen, rout);
      else
	error (ErrorError, "Local label with routine name %.*s does not match with current routine name %s", (int)routLen, rout, curROUTId);
      if (fileP != NULL)
	errorLine (fileP, lineNum, ErrorError, "note: Last ROUT was here");
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
  if (!isdigit ((unsigned char)inputLook ()))
    {
      error (ErrorError, "Missing local label number");
      return UINT_MAX;
    }
  unsigned label = (unsigned char)inputGet () - '0';
  while (isdigit ((unsigned char)inputLook ()))
    {
      unsigned next_label = 10*label + (unsigned char)inputGet () - '0';
      if (next_label < label)
	{
	  error (ErrorError, "Local label overflow");
	  return UINT_MAX;
	}
      label = next_label;
    }

  /* If a routinename is given, check if thats the one given with ROUT.  */
  size_t routLen;
  const char *rout = inputSymbol (&routLen, '\0');
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
	  error (ErrorError, "Local label overflow");
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
  Lex result =
    {
      .tag = LexNone
    };

  unsigned label = Lex_ReadReferringLocalLabel ();
  if (label == UINT_MAX)
    return result;

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
	  errorAbort ("Missing local label %%b%s%i", levelStr, label);
	  return result;
	}
      Local_CreateSymbolForOutstandingFwdLabelRef (id, sizeof (id),
						   level, dir, label);
    }
  else
    Local_CreateSymbol (lblP, macroDepth, false, id, sizeof (id));

  result.tag = LexId;
  result.Data.Id.str = strdup (id);
  if (!result.Data.Id.str)
    errorOutOfMem ();
  result.Data.Id.len = strlen (id);
  result.Data.Id.hash = lexHashStr (result.Data.Id.str, result.Data.Id.len);
  return result;
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
  nextbinopvalid = false; /* FIXME: why would this be necessary ? */

  if (isdigit ((unsigned char)inputLook ()))
    {
      /* Looks like this is a local label.  We just turn this into a
         LexLocalLabel Lex object and only when it is going to be used as
         a defining label (i.e. Lex_DefineLocalLabel), we'll check if this
	 is a valid local label.  */
      const char *beginLabel = Input_GetMark ();
      while (isdigit ((unsigned char)inputLook ()))
	(void) inputGet ();
      /* Possibly followed by a ROUT identifier.  */
      size_t routLen;
      (void) inputSymbol (&routLen, '\0');

      Lex result =
	{
	  .tag = LexLocalLabel,
	  .Data.LocalLabel.len = Input_GetMark () - beginLabel,
	  .Data.LocalLabel.str = beginLabel
	};
      return result;
    }

  return lexGetId ();
}


/**
 * Tries to turn a LexLocalLabel object into a local label symbol.
 * \return LexId which can be used to create a label symbol.  Can also
 * be LexNone in case of an error (like malformed local label, or wrong
 * routine name).
 * FIXME: should be removed as it leaks memory, teach Symbol_Get() to accept LexLocalLabel.
 */
Lex
Lex_DefineLocalLabel (const Lex *lexP)
{
  assert (lexP->tag == LexLocalLabel);

  unsigned labelNum = Lex_ReadDefiningLocalLabel (lexP->Data.LocalLabel.str, lexP->Data.LocalLabel.len);
  if (labelNum == UINT_MAX)
    {
      const Lex noLex = { .tag = LexNone };
      return noLex;
    }
  
  Local_Label_t *lblP = Local_DefineLabel (labelNum);
  char id[256];
  Local_CreateSymbol (lblP, FS_GetMacroDepth (), true, id, sizeof (id));
  lblP->instance++;

  Lex result;
  result.tag = LexId;
  result.Data.Id.str = strdup (id);
  if (!result.Data.Id.str)
    errorOutOfMem ();
  result.Data.Id.len = strlen (id);
  result.Data.Id.hash = lexHashStr (result.Data.Id.str, result.Data.Id.len);
  return result;
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
  if (isdigit ((unsigned char)inputLook ()))
    {
      /* Looks like this is a local label.  */
      do
	{
	  (void) inputGet ();
	} while (isdigit ((unsigned char)inputLook ()));

      size_t len;
      (void) inputSymbol (&len, '\0');
      return true;
    }

  return lexGetId ().tag != LexNone;
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
  char c1 = inputGet ();
  switch (c1)
    {
      case 'B':
	FINISH_STR ("ASE:", eOp_Base, kPrioOp_Unary); /* :BASE: */
	break;

      case 'C':
	{
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'C':
		FINISH_STR ("_ENCODING:", eOp_CCEnc, kPrioOp_Unary); /* :CC_ENCODING: */
		break;
	      case 'H':
		FINISH_STR ("R:", eOp_Chr, kPrioOp_Unary); /* :CHR: */
		break;
	    }
	  inputUnGet (c2);
	  break;
	}

      case 'D':
	if (Input_MatchString ("EF:")) /* :DEF: */
	  {
	    *lex = lexGetPrim ();
	    if (lex->tag == LexId)
	      {
	        /* :DEF: only returns {TRUE} when the symbol is defined and it is
		   not a macro local variable.  */
		const Symbol *symP = Symbol_Find (lex);
		lex->Data.Bool.value = symP != NULL && !(symP->type & SYMBOL_MACRO_LOCAL);
		lex->tag = LexBool;
		return;
	      }
	    else
	      error (ErrorError, "Bad operand for :DEF:");
	  }
	break;

      case 'F':
	{
	  char c2 = inputGet ();
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
	  inputUnGet (c2);
	  break;
	}
	
      case 'I':
	FINISH_STR ("NDEX:", eOp_Index, kPrioOp_Unary); /* :INDEX: */
	break;

      case 'L':
	{
	  char c2 = inputGet ();
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
	  inputUnGet (c2);
	  break;
	}
	
      case 'N':
	FINISH_STR ("OT:", eOp_Not, kPrioOp_Unary); /* :NOT: */
	break;

      case 'R':
	{
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'C':
		FINISH_STR ("ONST:", eOp_RConst, kPrioOp_Unary); /* :RCONST: */
		break;
	      case 'E':
		FINISH_STR ("VERSE_CC:", eOp_RevCC, kPrioOp_Unary); /* :REVERSE_CC: */
		break;
	    }
	  inputUnGet (c2);
	  break;
	}

      case 'S':
	FINISH_STR ("TR:", eOp_Str, kPrioOp_Unary); /* :STR: */
	break;

      case 'U':
	FINISH_STR ("PPERCASE:", eOp_UpperCase, kPrioOp_Unary); /* :UPPERCASE: */
	break;
    }
  inputUnGet (c1);

  /* Failed to tokenize.  */
  lex->tag = LexNone;
}


static void
Lex_GetBinaryOp (Lex *lex)
{
  lex->tag = LexOperator;
  char c1 = inputGet ();
  switch (c1)
    {
      case 'A':
	{
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'N':
		FINISH_STR ("D:", eOp_And, kPrioOp_AddAndLogical); /* :AND: */
		break;
	      case 'S':
		FINISH_STR ("R:", eOp_ASR, kPrioOp_Shift); /* :ASR: */
		break;
	    }
	  inputUnGet (c2);
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
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'A':
		FINISH_STR ("ND:", eOp_LAnd, kPrioOp_Boolean); /* :LAND: */
		break;
	      case 'E':
		{
		  char c3 = inputGet ();
		  switch (c3)
		    {
		      case 'F':
			FINISH_STR ("T:", eOp_Left, kPrioOp_String); /* :LEFT: */
			break;
		      case 'O':
			FINISH_STR ("R:", eOp_LEOr, kPrioOp_Boolean); /* :LEOR: */
			break;
		    }
		  inputUnGet (c3);
		  break;
		}
	      case 'O':
		FINISH_STR ("R:", eOp_LOr, kPrioOp_Boolean); /* :LOR: */
		break;
	    }
	  inputUnGet (c2);
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
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'I':
		FINISH_STR ("GHT:", eOp_Right, kPrioOp_String); /* :RIGHT: */
		break;
	      case 'O':
		{
		  char c3 = inputGet ();
		  switch (c3)
		    {
		      case 'L':
			FINISH_CHR (eOp_ROL, kPrioOp_Shift); /* :ROL: */
			break;
		      case 'R':
			FINISH_CHR (eOp_ROR, kPrioOp_Shift); /* :ROR: */
			break;
		    }
		  inputUnGet (c3);
		  break;
		}
	    }
	  inputUnGet (c2);
	  break;
	}
	
      case 'S':
	{
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'H':
		{
		  char c3 = inputGet ();
		  switch (c3)
		    {
		      case 'L':
			FINISH_CHR (eOp_SHL, kPrioOp_Shift); /* :SHL: */
			break;
		      case 'R':
			FINISH_CHR (eOp_SHR, kPrioOp_Shift); /* :SHR: */
			break;
		    }
		  inputUnGet (c3);
		  break;
		}
	    }
	  inputUnGet (c2);
	  break;
	}
    }
  inputUnGet (c1);

  /* Failed to tokenize.  */
  lex->tag = LexNone;
}


/**
 * Get built-in variable.
 * Built-in variables can be in uppercase, lowercase, or mixed.
 */
static void
Lex_GetBuiltinVariable (Lex *lex)
{
  lex->tag = LexNone;
  const char * const inputMark = Input_GetMark ();
  switch (inputGet () | 0x20)
    {
      case 'a':
	{
	  if (Input_MatchStringLower ("sasm}")) /* {ASASM} */ /* FIXME: change this into {asasm_version} = |asasm$version| = PVtbbbb (P = major version, V = minor version, t = patch release, bbbb = build number) */
	    {
	      lex->tag = LexBool;
	      lex->Data.Bool.value = true;
	      return;
	    }
	  else if (Input_MatchStringLower ("rchitecture}")) /* {ARCHITECTURE} */
	    {
	      lex->tag = LexString;
	      lex->Data.String.str = Target_GetArch (true);
	      lex->Data.String.len = strlen (lex->Data.String.str);
	      return;
	    }
	  else if (Input_MatchStringLower ("reaname}")) /* {AREANAME} */
	    {
	      lex->tag = LexString;
	      lex->Data.String.str = areaCurrentSymbol->str;
	      lex->Data.String.len = areaCurrentSymbol->len;
	      return;
	    }
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
	      lex->tag = LexInt;
	      lex->Data.Int.value = State_GetInstrType () == eInstrType_ARM ? 32 : 16;
	      return;
	    }
	  else if (Input_MatchStringLower ("onfig}")) /* {CONFIG} */
	    {
	      lex->tag = LexInt;
	      lex->Data.Int.value = State_GetInstrType () == eInstrType_ARM ? ((gOptionAPCS & APCS_OPT_32BIT) ? 32 : 26) : 16;
	      return;
	    }
	  else if (Input_MatchStringLower ("pu}")) /* {CPU} */
	    {
	      lex->tag = LexString;
	      lex->Data.String.str = Target_GetCPU ();
	      lex->Data.String.len = strlen (lex->Data.String.str);
	      return;
	    }
	  break;
	}

      case 'e':
	{
	  if (Input_MatchStringLower ("ndian}")) /* {ENDIAN} */
	    {
	      lex->tag = LexString;
	      lex->Data.String.str = "little";
	      lex->Data.String.len = sizeof ("little")-1;
	      return;
	    }
	  break;
	}

      case 'f':
	{
	  if (Input_MatchStringLower ("alse}")) /* {FALSE} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Int.value = false;
	      return;
	    }
	  else if (Input_MatchStringLower ("pu}")) /* {FPU} */
	    {
	      lex->tag = LexString;
	      lex->Data.String.str = Target_GetFPU ();
	      lex->Data.String.len = strlen (lex->Data.String.str);
	      return;
	    }
	  break;
	}

      case 'i':
	{
	  if (Input_MatchStringLower ("nputfile}")) /* {INPUTFILE} */
	    {
	      lex->tag = LexString;
	      lex->Data.String.str = FS_GetCurFileName ();
	      lex->Data.String.len = strlen (lex->Data.String.str);
	      return;
	    }
	  if (Input_MatchStringLower ("nter}")) /* {INTER} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Bool.value = (gOptionAPCS & APCS_OPT_INTERWORK) != 0;
	      return;
	    }
	  break;
	}

      case 'l':
	{
	  if (Input_MatchStringLower ("inenum}")) /* {LINENUM} */
	    {
	      lex->tag = LexInt;
	      lex->Data.Int.value = FS_GetBuiltinVarLineNum ();
	      return;
	    }
	  else if (Input_MatchStringLower ("inenumup}")) /* {LINENUMUP} */
	    {
	      lex->tag = LexInt;
	      lex->Data.Int.value = FS_GetBuiltinVarLineNumUp ();
	      return;
	    }
	  else if (Input_MatchStringLower ("inenumupper}")) /* {LINENUMUPPER} */
	    {
	      lex->tag = LexInt;
	      lex->Data.Int.value = FS_GetBuiltinVarLineNumUpper ();
	      return;
	    }
	  break;
	}

      case 'o':
	{
	  if (Input_MatchStringLower ("pt}")) /* {OPT} */
	    {
	      lex->tag = LexInt;
	      lex->Data.Int.value = gOpt_DirectiveValue;
	      return;
	    }
	  break;
	}

      case 'p':
	{
	  /* FIXME: {PCSTOREOFFSET} */
	  if (Input_MatchStringLower ("c}")) /* {PC} */
	    {
	      lex->tag = LexPosition;
	      return;
	    }
	  break;
	}

      case 'r':
	{
	  if (Input_MatchStringLower ("opi}")) /* {ROPI} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Bool.value = (gOptionAPCS & APCS_OPT_ROPI) != 0;
	      return;
	    }
	  if (Input_MatchStringLower ("eentrant}") /* {REENTRANT} */
	      || Input_MatchKeywordLower ("wpi}")) /* {RWPI} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Bool.value = (gOptionAPCS & APCS_OPT_REENTRANT) != 0;
	      return;
	    }
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
		      lex->tag = LexInt;
		      lex->Data.Int.value = Target_GetARMISAVersion ();
		      return;
		    }
		  if (Input_MatchStringLower ("thumb}")) /* {TARGET_ARCH_THUMB} */
		    {
		      /* Number Thumb architecture, or 0 if ARM-only ISA.  */
		      lex->tag = LexInt;
		      lex->Data.Int.value = Target_GetThumbISAVersion ();
		      return;
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
      		      { "7e_m}", "7E-M" }
		    };
		  for (size_t i = 0; i != sizeof (oArchs)/sizeof (oArchs[0]); ++i)
		    {
		      if (Input_MatchStringLower (oArchs[i].arch_in)) /* {TARGET_ARCH_<architecture>} */
			{
			  lex->tag = LexBool;
			  lex->Data.Bool.value = !strcmp (Target_GetArch (false), oArchs[i].arch_norm);
			  return;
			}
		    }
		}
	      else if (Input_MatchStringLower ("feature_"))
		{
		  if (Input_MatchStringLower ("clz}")) /* {TARGET_FEATURE_CLZ} */
		    {
		      /* True when CLZ is available in Thumb *or* ARM.
		          So needs at least v5 architecture (ARM) or Thumb v4 (Thumb).  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = Target_GetARMISAVersion () >= 5
					       || Target_GetThumbISAVersion () >= 4;
		      return;
		    }
		  if (Input_MatchStringLower ("divide}")) /* {TARGET_FEATURE_DIVIDE} */
		    {
		      /* True when SDIV/UDIV is available in Thumb *or* ARM.  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = (Target_GetCPUFeatures () & kCPUExt_Div) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("doubleword}")) /* {TARGET_FEATURE_DOUBLEWORD} */
		    {
		      /* True when LDRD/STRD is available (P extension).
			 So needs at least v5E architecture but not for ARMv6-M.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetCPUFeatures ();
		      lex->Data.Bool.value = (features & kCPUExt_v5E) != 0
					       && ((features & (kCPUExt_v6M | kCPUExt_v7M)) != kCPUExt_v6M); 
		      return;
		    }
		  if (Input_MatchStringLower ("dspmul}")) /* {TARGET_FEATURE_DSPMUL} */
		    {
		      /* True when DSP & saturated math is available (E extension).  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = (Target_GetCPUFeatures () & kCPUExt_v5ExP) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("extension_register_count}")) /* {TARGET_FEATURE_EXTENSION_REGISTER_COUNT} */
		    {
		      lex->tag = LexInt;
		      unsigned features = Target_GetFPUFeatures () ;
		      if ((features & (kFPUExt_VFPv1 | kFPUExt_VFPv2 | kFPUExt_VFPv3)) != 0)
			lex->Data.Int.value = (features & kFPUExt_D16D32) ? 32 : 16;
		      else
			lex->Data.Int.value = 0;
		      return;
		    }
		  if (Input_MatchStringLower ("multiply}")) /* {TARGET_FEATURE_MULTIPLY} */
		    {
		      /* True when long multiply instructions are available in
			 Thumb *or* ARM (M extension).  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetCPUFeatures ();
		      lex->Data.Bool.value = (features & (kCPUExt_v3M | kCPUExt_v7M)) != 0
					       || features == kArchExt_v7;
		      return;
		    }
		  if (Input_MatchStringLower ("multiprocessing}")) /* {TARGET_FEATURE_MULTIPROCESSING} */
		    {
		      /* True when multiprocessing extensions are supported.  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = (Target_GetCPUFeatures () & kCPUExt_MP) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("neon}")) /* {TARGET_FEATURE_NEON} */
		    {
		      /* True when there is NEON support.  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = (Target_GetFPUFeatures () & kFPUExt_NEONv1) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("neon_fp16}")) /* {TARGET_FEATURE_NEON_FP16} */
		    {
		      /* True when there is half-precision NEON support.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value =  (features & (kFPUExt_NEONv1 | kFPUExt_FP16)) == (kFPUExt_NEONv1 | kFPUExt_FP16);
		      return;
		    }
		  if (Input_MatchStringLower ("neon_fp32}")) /* {TARGET_FEATURE_NEON_FP32} */
		    {
		      /* True when there is single-precision NEON support.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & kFPUExt_NEONv1) != 0
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("neon_integer}")) /* {TARGET_FEATURE_NEON_INTEGER} */
		    {
		      /* True when there is NEON integer-only support (i.e. NEON but no VFP at all).  */ 
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & kFPUExt_NEONv1) != 0
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == 0;
		      return;
		    }
		  if (Input_MatchStringLower ("unaligned}")) /* {TARGET_FEATURE_UNALIGNED} */
		    {
		      /* True when unaligned access is supported.
		         From ARMv6 onwards but not for ARMv6-M.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetCPUFeatures ();
		      lex->Data.Bool.value = (features & kCPUExt_v6) != 0
					       && (features & (kCPUExt_v6M | kCPUExt_v7M)) != kCPUExt_v6M; 
		      return;
		    }
		}
	      else if (Input_MatchStringLower ("fpu_"))
		{
		  if (Input_MatchStringLower ("fpa}")) /* {TARGET_FPU_FPA} */
		    {
		      /* True when FPA FPU is selected.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures () ;
		      lex->Data.Bool.value = (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == 0
					       && (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("softfpa}")) /* {TARGET_FPU_SOFTFPA} */
		    {
		      /* True when FPA endian + float-abi == soft.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures () ;
		      lex->Data.Bool.value = (features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2 | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == kFPUExt_SoftFPA;  
		      return;
		    }
		  if (Input_MatchStringLower ("softfpa_fpa}")) /* {TARGET_FPU_SOFTFPA_FPA} */
		    {
		      /* True when FPA endian + float-abi == softfp (using FPA).  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures () ;
		      lex->Data.Bool.value = (features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == kFPUExt_SoftFPA
					       && (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) != 0;  
		      return;
		    }
		  if (Input_MatchStringLower ("softfpa_vfp}")) /* {TARGET_FPU_SOFTFPA_VFP} */
		    {
		      /* True when FPA endian + float-abi == softfp (using VFP).  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures () ;
		      lex->Data.Bool.value = (features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2)) == kFPUExt_SoftFPA
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0;  
		      return;
		    }
		  if (Input_MatchStringLower ("softvfp}")) /* {TARGET_FPU_SOFTVFP} */
		    {
		      /* True when VFP endian + float-abi == soft.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures () ;
		      lex->Data.Bool.value = (features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2 | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == (kFPUExt_NoEndianMismatch | kFPUExt_SoftVFP);  
		      return;
		    }
		  if (Input_MatchStringLower ("softvfp_fpa}")) /* {TARGET_FPU_SOFTVFP_FPA} */
		    {
		      /* True when VFP endian + float-abi == softfp (using FPA).  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures () ;
		      lex->Data.Bool.value = (features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == (kFPUExt_NoEndianMismatch | kFPUExt_SoftVFP)
					       && (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) != 0;  
		      return;
		    }
		  if (Input_MatchStringLower ("softvfp_vfp}")) /* {TARGET_FPU_SOFTVFP_VFP} */
		    {
		      /* True when VFP endian + float-abi == softfp (using VFP).  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & (kFPUExt_NoEndianMismatch | kFPUExt_SoftFPA | kFPUExt_SoftVFP | kFPUExt_FPAv1 | kFPUExt_FPAv2)) == (kFPUExt_NoEndianMismatch | kFPUExt_SoftVFP)
			                       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0;  
		      return;
		    }
		  if (Input_MatchStringLower ("vfp}")) /* {TARGET_FPU_VFP} */
		    {
		      /* True when VFP FPU is selected.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("vfpv1}")) /* {TARGET_FPU_VFPV1} */
		    {
		      /* True when VFPv1 is selected. AsAsm extension.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_VFP_FMA)) == kFPUExt_VFPv1xD;
		      return;
		    }
		  if (Input_MatchStringLower ("vfpv2}")) /* {TARGET_FPU_VFPV2} */
		    {
		      /* True when VFPv2 is selected.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_VFP_FMA)) == (kFPUExt_VFPv1xD | kFPUExt_VFPv2);
		      return;
		    }
		  if (Input_MatchStringLower ("vfpv3}")) /* {TARGET_FPU_VFPV3} */
		    {
		      /* True when VFPv3 is selected.  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) == (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA)) != (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA);
		      return;
		    }
		  if (Input_MatchStringLower ("vfpv4}")) /* {TARGET_FPU_VFPV4} */
		    {
		      /* True when VFPv4 is selected (i.e. VFPv3 + FP16 + VFP_FMA).  */
		      lex->tag = LexBool;
		      unsigned features = Target_GetFPUFeatures ();
		      lex->Data.Bool.value = (features & (kFPUExt_FPAv1 | kFPUExt_FPAv2)) == 0
					       && (features & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA)) == (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA);
		      return;
		    }
		}
	      else if (Input_MatchStringLower ("profile_"))
		{
		  if (Input_MatchStringLower ("a}")) /* {TARGET_PROFILE_A} */
		    {
		      /* True for ARMv7-A.  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = (Target_GetCPUFeatures () & kCPUExt_v7A) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("m}")) /* {TARGET_PROFILE_M} */
		    {
		      /* True for ARMv6-M or ARMv7-M.  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = (Target_GetCPUFeatures () & (kCPUExt_v6M | kCPUExt_v7M)) != 0;
		      return;
		    }
		  if (Input_MatchStringLower ("r}")) /* {TARGET_PROFILE_R} */
		    {
		      /* True for ARMv7-R.  */
		      lex->tag = LexBool;
		      lex->Data.Bool.value = (Target_GetCPUFeatures () & kCPUExt_v7R) != 0;
		      return;
		    }
		}
	    }
	  else if (Input_MatchStringLower ("rue}")) /* {TRUE} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Int.value = true;
	      return;
	    }
	  break;
	}

      case 'u':
	{
	  if (Input_MatchStringLower ("al}")) /* {UAL} */
	    {
	      /* True when we only going to parse UAL.  */
	      lex->tag = LexBool;
	      lex->Data.Bool.value = false; /* FIXME: */
	      return;
	    }
	  break;
	}
      case 'v':
	{
	  if (Input_MatchStringLower ("ar}")) /* {VAR} */
	    {
	      lex->tag = LexStorage;
	      return;
	    }
	  break;
	}
    }

  /* Try to find the end of the built-in variable name.  */
  const char *lineRest = Input_Rest ();
  while (*lineRest != '\0' && *lineRest != '\n' && *lineRest != '}')
    ++lineRest;
  if (*lineRest == '}')
    error (ErrorError, "Unknown built-in variable {%.*s",
	   (int)(lineRest + 1 - inputMark), inputMark);
  else
    error (ErrorError, "Missing closing bracket");
  lex->tag = LexNone;
}


static ARMWord
Lex_Char2Int (size_t len, const char *str)
{
  assert (len <= 4);

  ARMWord result = 0;
  for (size_t i = 0; i != 4; ++i)
    {
      result >>= 8;
      if (i < len)
	result |= (unsigned)str[i] << 24;
    }
  return result;
}


/**
 * Parse floating point literal starting with "0f_" + followed by exactly
 * 8 hex digits.
 * The initial '0' is already parsed.  The "f_" are already checked but not
 * yet consumed.
 */
static void
Lex_GetFloatFloatingPointLiteral (Lex *result)
{
  inputSkipN (sizeof ("f_")-1); /* Skip "f_" */
  uint32_t fltAsInt = 0;
  for (int i = 0; i != 8; inputSkip (), ++i)
    {
      char c = inputLookLower ();
      if (!isxdigit (c))
	{
	  error (ErrorError, "Float floating point literal needs exactly 8 hex digits");
	  result->tag = LexNone;
	  return;
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

  result->tag = LexFloat;
  result->Data.Float.value = (double)flt.f;
}


/**
 * Parse floating point literal starting with "0d_" + followed by exactly
 * 16 hex digits.
 * The initial '0' is already parsed.  The "d_" are already checked but not
 * yet consumed.
 */
static void
Lex_GetDoubleFloatingPointLiteral (Lex *result)
{
  inputSkipN (2); /* Skip "f_" */
  uint64_t fltAsInt = 0;
  for (int i = 0; i != 16; inputSkip (), ++i)
    {
      char c = inputLookLower ();
      if (!isxdigit (c))
	{
	  error (ErrorError, "Double floating point literal needs exactly 16 hex digits");
	  result->tag = LexNone;
	  return;
	}
      fltAsInt = 16*fltAsInt + c - ((c >= 'a') ? 'a' - 10 : '0');
    }
  /* FIXME: VFP support missing ! */
  const union arm_double_fpa armdbl_fpa = { .i = fltAsInt };
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

  result->tag = LexFloat;
  result->Data.Float.value = dbl.d;
}


Lex
lexGetPrim (void)
{
  Lex result;

  nextbinopvalid = false;
  skipblanks ();
  char c = inputGet ();
  switch (c)
    {
      case '+':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_None; /* +XYZ */
	result.Data.Operator.pri = kPrioOp_Unary;
	break;

      case '-':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_Neg; /* -XYZ */
	result.Data.Operator.pri = kPrioOp_Unary;
	break;

      case '!':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_LNot; /* !XYZ */
	result.Data.Operator.pri = kPrioOp_Unary;
	break;

      case '~':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_Not; /* ~XYZ */
	result.Data.Operator.pri = kPrioOp_Unary;
	break;

      case '?':
	result.tag = LexOperator;
	result.Data.Operator.op = eOp_Size; /* ?<label> */
	result.Data.Operator.pri = kPrioOp_Unary;
	break;

      case '(':
      case ')':
	result.tag = LexDelim;
	result.Data.Delim.delim = c;
	break;

      case ':':
	Lex_GetUnaryOp (&result);
	if (result.tag == LexNone)
	  inputUnGet (':');
	break;

      case '&':
	{
	  bool didOverflow;
	  result = Lex_GetInt (16, &didOverflow);
	  if (didOverflow)
	    error (ErrorWarning, "64-bit integer overflow");
	  break;
	}

      case '0':
	{
	  /* Floating point literal 0f_... or 0d_... ? */
	  if (inputLookN (0) == 'f' && inputLookN (1) == '_')
	    {
	      Lex_GetFloatFloatingPointLiteral (&result);
	      break;
	    }
	  if (inputLookN (0) == 'd' && inputLookN (1) == '_')
	    {
	      Lex_GetDoubleFloatingPointLiteral (&result);
	      break;
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
	  inputUnGet (c);
	  const char *mark = Input_GetMark ();
	  bool didOverflow;
	  result = Lex_GetInt (10, &didOverflow);
	  if (Input_Match ('.', false))
	    {
	      Input_RollBackToMark (mark);
	      result.tag = LexFloat;
	      char *newMark;
	      result.Data.Float.value = strtod (mark, &newMark);
	      if (newMark == mark)
		error (ErrorError, "Failed to read floating point value");
	      inputSkipN (newMark - mark);
	    }
	  else if (didOverflow)
	    error (ErrorWarning, "64-bit integer overflow");
	  break;
	}

      case '\'':
	{
	  char in[4];
	  int i, ci;
	  for (i = 0; (ci = (unsigned char)inputGet ()) != '\''; ++i)
	    {
	      if (ci == '\\')
		ci = inputGet ();
	      if (ci == '\0')
		{
		  error (ErrorError, "Constant specification continues over newline");
		  break;
		}
	      if (i == 4)
		{
		  error (ErrorError, "Illegal constant specification");
		  break;
		}
	      in[i] = ci;
	    }

	  result.tag = LexInt;
	  result.Data.Int.value = Lex_Char2Int (i, in);
	  break;
	}

      case '"':
	{
	  result.tag = LexString;
	  result.Data.String.str = Input_GetString (&result.Data.String.len); /* FIXME: this leaks memory.  */
	  break;
	}

      case '.':
	{
	  /* Do we have the position mark '.' or start of a floating point
	     number ? */
	  if (isdigit ((unsigned char)inputLook ()))
	    {
	      /* Looks like a floating point number.  */
	      inputUnGet (c);
	      const char *mark = Input_GetMark ();
	      result.tag = LexFloat;
	      char *newMark;
	      result.Data.Float.value = strtod (mark, &newMark);
	      if (newMark == mark)
		error (ErrorError, "Failed to read floating point value");
	      inputSkipN (newMark - mark);
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
	  switch (inputLookLower ())
	    {
	      case 'f':
		/* Forward looking.  */
		inputSkip ();
		dir = eForward;
		break;
  
	      case 'b':
		/* Backward looking.  */
		inputSkip ();
		dir = eBackward;
		break;
  
	      default:
		/* Search backwards first, then forward.  */
		dir = eBackwardThenForward;
		break;
	    }
	  LocalLabel_eLevel level;
	  switch (inputLookLower ())
	    {
	      case 't':
		/* Only at this macro level.  */
		inputSkip ();
		level = eThisLevelOnly;
		break;
  
	      case 'a':
		/* All macro levels.  */
		inputSkip ();
		level = eAllLevels;
		break;
  
	      default:
		/* At this macro level and all upper levels.  */
		level = eThisLevelAndHigher;
		break;
	    }
	  result = Lex_MakeReferringLocalLabel (dir, level);
	  break;
	}

      case '{':
	Lex_GetBuiltinVariable (&result);
	break;

      default:
	/* Try to read a symbol.  */
	inputUnGet (c);
	if ((result.Data.Id.str = Input_Symbol (&result.Data.Id.len)) != NULL)
	  {
	    result.Data.Id.hash = lexHashStr (result.Data.Id.str, result.Data.Id.len);
	    result.tag = LexId;
	  }
	else
	  result.tag = LexNone;
	break;
    }

  return result;
}

Lex
lexGetBinop (void)
{
  if (nextbinopvalid)
    {
      nextbinopvalid = false;
      return nextbinop;
    }

  skipblanks ();
  Lex result;
  int c;
  switch (c = inputGet ())
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
	switch (inputLook ())
	  {
	    case '>':
	      result.Data.Operator.pri = kPrioOp_Shift;
	      inputSkip ();
	      if (Input_Match ('>', false))
		result.Data.Operator.op = eOp_ASR; /* >>> */
	      else
		result.Data.Operator.op = eOp_SHR; /* >> */
	      break;

	    case '=':
	      inputSkip ();
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
	switch (inputLook ())
	  {
	    case '<':
	      inputSkip ();
	      result.Data.Operator.op = eOp_SHL; /* << */
	      result.Data.Operator.pri = kPrioOp_Shift;
	      break;
    
	    case '=':
	      inputSkip ();
	      result.Data.Operator.op = eOp_LE; /* <= */
	      result.Data.Operator.pri = kPrioOp_Relational;
	      break;
    
	    case '>':
	      inputSkip ();
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
	    inputUnGet (c);
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
	  inputUnGet (':');
	break;

      default:
	inputUnGet (c);
	result.tag = LexNone;
	break;
    }

  return result;
}

int
lexNextPri (void)
{
  if (!nextbinopvalid)
    {
      nextbinop = lexGetBinop ();
      nextbinopvalid = true;
    }
  return (nextbinop.tag == LexOperator) ? nextbinop.Data.Operator.pri : -1;
}

Lex
lexTempLabel (const char *str, size_t len)
{
  const Lex var =
    {
      .tag = LexId,
      .Data.Id = { .str = str, .len = len, .hash = lexHashStr (str, len) }
    };
  return var;
}

#ifdef DEBUG
void
lexPrint (const Lex *lex)
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
	printf ("Str <%.*s> ", (int)lex->Data.String.len, lex->Data.String.str);
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
	printf ("Op <%d, %d> ", lex->Data.Operator.op, lex->Data.Operator.pri);
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
	printf("bool <%d> ", lex->Data.Bool.value);
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
      ":LNOT:",		/* eOp_lnot */
      ":NOT:",		/* eOp_Not */
      ":NEG:",		/* eOp_Neg */
      ":NONE:",		/* eOp_None */
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
