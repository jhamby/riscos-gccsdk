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
 * lexAcorn.c
 */

#include "config.h"

#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>

#include "area.h"
#include "common.h"
#include "decode.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "lex.h"
#include "lexAcorn.h"
#include "main.h"
#include "opt.h"
#include "state.h"
#include "targetcpu.h"

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

void
lexAcornUnop (Lex *lex)
{
  lex->tag = LexOperator;
  char c1 = inputGet ();
  switch (c1)
    {
      case 'B':
	FINISH_STR ("ASE:", Op_base, kPrioOp_Unary); /* :BASE: */
	break;

      case 'C':
	FINISH_STR ("HR:", Op_chr, kPrioOp_Unary); /* :CHR: */
	break;

      case 'D':
	if (Input_MatchString ("EF:")) /* :DEF: */
	  {
	    *lex = lexGetPrim ();
	    if (lex->tag == LexId)
	      {
	        /* :DEF: only returns {TRUE} when the symbol is defined and it is
		   not a macro local variable.  */
		const Symbol *symP = symbolFind (lex);
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
		FINISH_STR ("TTR:", Op_fattr, kPrioOp_Unary); /* :FATTR: */
		break;
	      case 'E':
		FINISH_STR ("XEC:", Op_fexec, kPrioOp_Unary); /* :FEXEC: */
		break;
	      case 'L':
		FINISH_STR ("OAD:", Op_fload, kPrioOp_Unary); /* :FLOAD: */
		break;
	      case 'S':
		FINISH_STR ("IZE:", Op_fsize, kPrioOp_Unary); /* :FSIZE: */
		break;
	    }
	  inputUnGet (c2);
	  break;
	}
	
      case 'I':
	FINISH_STR ("NDEX:", Op_index, kPrioOp_Unary); /* :INDEX: */
	break;

      case 'L':
	{
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'E':
		FINISH_STR ("N:", Op_len, kPrioOp_Unary); /* :LEN: */
		break;
	      case 'N':
		FINISH_STR ("OT:", Op_lnot, kPrioOp_Unary); /* :LNOT: */
		break;
	    }
	  inputUnGet (c2);
	  break;
	}
	
      case 'N':
	FINISH_STR ("OT:", Op_not, kPrioOp_Unary); /* :NOT: */
	break;

      case 'S':
	FINISH_STR ("TR:", Op_str, kPrioOp_Unary); /* :STR: */
	break;
    }
  inputUnGet (c1);

  /* Failed to tokenize.  */
  lex->tag = LexNone;
}


void
lexAcornBinop (Lex *lex)
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
		FINISH_STR ("D:", Op_and, kPrioOp_AddAndLogical); /* :AND: */
		break;
	      case 'S':
		FINISH_STR ("R:", Op_asr, kPrioOp_Shift); /* :ASR: */
		break;
	    }
	  inputUnGet (c2);
	  break;
	}
	
      case 'C':
	FINISH_STR ("C:", Op_concat, kPrioOp_String); /* :CC: */
	break;
	
      case 'E':
	FINISH_STR ("OR:", Op_xor, kPrioOp_AddAndLogical); /* :EOR: */
	break;
	
      case 'L':
	{
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'A':
		FINISH_STR ("ND:", Op_land, kPrioOp_Boolean); /* :LAND: */
		break;
	      case 'E':
		{
		  char c3 = inputGet ();
		  switch (c3)
		    {
		      case 'F':
			FINISH_STR ("T:", Op_left, kPrioOp_String); /* :LEFT: */
			break;
		      case 'O':
			FINISH_STR ("R:", Op_leor, kPrioOp_Boolean); /* :LEOR: */
			break;
		    }
		  inputUnGet (c3);
		  break;
		}
	      case 'O':
		FINISH_STR ("R:", Op_lor, kPrioOp_Boolean); /* :LOR: */
		break;
	    }
	  inputUnGet (c2);
	  break;
	}
	
      case 'M':
	FINISH_STR ("OD:", Op_mod, kPrioOp_Multiplicative); /* :MOD: */
	break;

      case 'O':
	FINISH_STR ("R:", Op_or, kPrioOp_AddAndLogical); /* :OR: */
	break;

      case 'R':
	{
	  char c2 = inputGet ();
	  switch (c2)
	    {
	      case 'I':
		FINISH_STR ("GHT:", Op_right, kPrioOp_String); /* :RIGHT: */
		break;
	      case 'O':
		{
		  char c3 = inputGet ();
		  switch (c3)
		    {
		      case 'L':
			FINISH_CHR (Op_rol, kPrioOp_Shift); /* :ROL: */
			break;
		      case 'R':
			FINISH_CHR (Op_ror, kPrioOp_Shift); /* :ROR: */
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
			FINISH_CHR (Op_sl, kPrioOp_Shift); /* :SHL: */
			break;
		      case 'R':
			FINISH_CHR (Op_sr, kPrioOp_Shift); /* :SHR: */
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
 * Get builtin variable.
 * Built-in variables can be in uppercase, lowercase, or mixed.
 */
void
lexAcornPrim (Lex *lex)
{
  lex->tag = LexNone;
  const char * const inputMark = Input_GetMark ();
  switch (inputGet () | 0x20)
    {
      case 'a':
	{
	  if (Input_MatchStringLower ("sasm}")) /* {ASASM} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Int.value = true;
	      return;
	    }
	  else if (Input_MatchStringLower ("rchitecture}")) /* {ARCHITECTURE} */
	    {
	      lex->tag = LexString;
	      lex->Data.String.str = Target_GetArchAsString ();
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
	  if (Input_MatchStringLower ("onfig}") || Input_MatchStringLower ("odesize}")) /* {CONFIG} or {CODESIZE} */
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
	  /* FIXME: support FPU */
	  if (Input_MatchStringLower ("alse}")) /* {FALSE} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Int.value = false;
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

      case 't':
	{
	  if (Input_MatchStringLower ("rue}")) /* {TRUE} */
	    {
	      lex->tag = LexBool;
	      lex->Data.Int.value = true;
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

  /* Try to find the end of the builtin variable name.  */
  const char *lineRest = Input_Rest ();
  while (*lineRest != '\0' && *lineRest != '\n' && *lineRest != '}')
    ++lineRest;
  if (*lineRest == '}')
    error (ErrorError, "Unknown builtin variable {%.*s",
	   (int)(lineRest + 1 - inputMark), inputMark);
  else
    error (ErrorError, "Missing closing bracket");
  lex->tag = LexNone;
}
