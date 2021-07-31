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
#include <stdint.h>

#include "error.h"
#include "input.h"
#include "main.h"
#include "option.h"
#include "state.h"
#include "targetcpu.h"

typedef struct
{
  size_t idx;
  ARMWord cc;
  bool doLowerCase;
} ParseState;

static ARMWord GetCCodeIfThere (bool doLowerCase, unsigned skipChars);


static void
ParseState_Init (ParseState *stateP, bool doLowerCase)
{
  stateP->idx = 0;
  stateP->cc = 0;
  stateP->doLowerCase = doLowerCase;
}


/**
 * Try to parse an optional character (like 'L'/'l', or 'E'/'e').
 * \param c Upcase character to match.
 */
static bool
ParseState_IsOptChar (ParseState *stateP, ARMWord value, char c)
{
  if (Input_LookN (stateP->idx) == (stateP->doLowerCase ? c | 32 : c))
    {
      stateP->cc |= value;
      ++stateP->idx;
    }
  return true;
}


/**
 * Try to parse optional condition code.
 */
static bool
ParseState_IsOptCC (ParseState *stateP)
{
  ARMWord cc = GetCCodeIfThere (stateP->doLowerCase, stateP->idx);
  if (cc != kOption_NotRecognized)
    {
      stateP->cc |= cc;
      stateP->idx += 2;
    }
  else
    stateP->cc |= AL;
  return true;
}


/**
 * Check whether we're at the end of a keyword (there might still be a
 * qualifier following).
 */
static bool
ParseState_IsEndOfKeyword (ParseState *stateP)
{
  if (Input_IsEndOfKeywordN (stateP->idx) || Input_LookN (stateP->idx) == '.')
    {
      Input_SkipN (stateP->idx);
      return true;
    }
  return false;
}


/**
 * Try to parse [<cc>][<c>] (pre-UAL) and/or [<c>][<cc>] (UAL) + end-of-keyword.
 * \param doLowerCase Whether the lowercase or uppercase version of c needs to
 * be parsed.
 * \param c Optional <c> to parse (uppercase).
 * \param cval Instruction value when <c> has been parsed.
 * \return kOption_NotRecognized when parsing failed.  Otherwise the
 * corresponding ARM instruction value for parsed elements.
 */
static ARMWord
GetOptCCodeAndOptChar (bool doLowerCase, char c, ARMWord cval)
{
  ParseState state;
  ParseState_Init (&state, doLowerCase);
  bool ok;
  if (!(ok = ParseState_IsOptCC (&state) && ParseState_IsEndOfKeyword (&state)))
    {
      ParseState_Init (&state, doLowerCase);
      switch (State_GetSyntax ())
	{
	  case eSyntax_PreUALOnly:
	    ok = ParseState_IsOptCC (&state) && ParseState_IsOptChar (&state, cval, c)
	           && ParseState_IsEndOfKeyword (&state);
	    break;

	  case eSyntax_Both:
	    ok = ParseState_IsOptCC (&state) && ParseState_IsOptChar (&state, cval, c)
	           && ParseState_IsEndOfKeyword (&state);
	    if (ok)
	      break;
	    ParseState_Init (&state, doLowerCase);
	    /* Fall through.  */

	  case eSyntax_UALOnly:
	    ok = ParseState_IsOptChar (&state, cval, c) && ParseState_IsOptCC (&state)
	           && ParseState_IsEndOfKeyword (&state);
	    break;
	}
    }
  return ok ? state.cc : kOption_NotRecognized;
}


/**
 * Try to parse a 2 character condition code.
 * \param doLowerCase When true, try to match on lowercase condition code,
 * uppercase otherwise.
 * \param skipChars Number of characters to skip before starting to match.
 * \return Parsed condition code.
 * When there is no condition code, kOption_NotRecognized is returned instead.
 */
static ARMWord
GetCCodeIfThere (bool doLowerCase, unsigned skipChars)
{
  ARMWord cc = kOption_NotRecognized;
  const char c1 = Input_LookN (skipChars + 0) ^ (doLowerCase ? 0 : 32);
  const char c2 = c1 != '\0' ? Input_LookN (skipChars + 1) ^ (doLowerCase ? 0 : 32) : '\0';
  switch (c1)
    {
      case 'a':
	{
	  if (c2 == 'l')
	    cc = AL;
	  break;
	}

      case 'c':
	{
	  if (c2 == 'c')
	    cc = CC;
	  else if (c2 == 's')
	    cc = CS;
	  break;
	}

      case 'e':
	{
	  if (c2 == 'q')
	    cc = EQ;
	  break;
	}

      case 'g':
	{
	  if (c2 == 'e')
	    cc = GE;
	  else if (c2 == 't')
	    cc = GT;
	  break;
	}
      case 'h':
	{
	  if (c2 == 'i')
	    cc = HI;
	  else if (c2 == 's')
	    cc = CS; /* HS == CS */
	  break;
	}

      case 'l':
	{
	  if (c2 == 'e')
	    cc = LE;
	  else if (c2 == 'o')
	    cc = CC; /* LO == CC */
	  else if (c2 == 's')
	    cc = LS;
	  else if (c2 == 't')
	    cc = LT;
	  break;
	}

      case 'm':
	{
	  if (c2 == 'i')
	    cc = MI;
	  break;
	}

      case 'n':
	{
	  if (c2 == 'e')
	    cc = NE;
	  else if (c2 == 'v')
	    {
	      /* Pre-ARMv4, NV is supported.
		 ARMv4, NV is UNPREDICTABLE.
		 ARMv5 and later, used for "Unconditional instruction extension space"  */
	      if (Target_CheckCPUFeature (kCPUExt_v5, false))
		{
		  /* cc already is kOption_NotRecognized.  */
		}
	      else if (Target_CheckCPUFeature (kCPUExt_v4, false))
		{
		  Error (ErrorWarning, "For ARMv4, use of NV condition code is UNPREDICTABLE");
		  cc = NV;
		}
	      else
		cc = NV;
	    }
	  break;
	}

      case 'p':
	{
	  if (c2 == 'l')
	    cc = PL;
	  break;
	}

      case 'v':
	{
	  if (c2 == 'c')
	    cc = VC;
	  else if (c2 == 's')
	    cc = VS;
	  break;
	}

      default:
	break;
    }

  return cc;
}


/**
 * Try to parse a 2 character condition code.
 * \param doLowerCase When true, try to match on lowercase condition code,
 * uppercase otherwise.
 * \return Parsed condition code.  When there is no condition code to parse,
 * kOption_NotRecognized is returned instead.
 */
ARMWord
Option_GetCCodeIfThere (bool doLowerCase)
{
  ARMWord cc = GetCCodeIfThere (doLowerCase, 0);
  if (cc != kOption_NotRecognized)
    Input_SkipN (2);
  return cc;
}


/**
 * Try to parse a 2 character condition code.
 * \return Parsed condition code.  When there is no condition code, AL
 * is returned instead.
 */
static ARMWord
GetCCode (bool doLowerCase)
{
  ARMWord ccode = Option_GetCCodeIfThere (doLowerCase);
  if (ccode == kOption_NotRecognized)
    ccode = AL;
  return ccode;
}


/**
 * When in non-pre-UAL only mode, try to read the instruction width indicator
 * (".W"/".w" or ".N"/".n") + real end-of-keyword.
 * When in pre-UAL only mode, only the real end-of-keyword is checked.
 * \param doLowerCase When true, instruction width indication should be
 * lowercase, uppercase otherwise.
 * \return Instruction qualifier when specified, or indication if there wasn't
 * any qualifier specified.  Narrow instruction qualifier will be faulted when
 * in ARM mode.
 */
InstrWidth_e
Option_GetInstrWidth (bool doLowerCase)
{
  if (State_GetSyntax () != eSyntax_PreUALOnly)
    {
      if (Input_MatchKeyword (doLowerCase ? ".w" : ".W"))
	return eInstrWidth_Enforce32bit;

      if (Input_MatchKeyword (doLowerCase ? ".n" : ".N"))
	{
	  if (State_GetInstrType () == eInstrType_ARM)
	    {
	      Error (ErrorError, "Narrow instruction qualifier is not possible in ARM mode");
	      return eInstrWidth_NotSpecified;
	    }

	  return eInstrWidth_Enforce16bit;
	}
    }

  return Input_IsEndOfKeyword () ? eInstrWidth_NotSpecified : eInstrWidth_Unrecognized;
}


/**
 * Try to read stackmode for LDM/STM/RFE/SRS instructions.
 * \return kOption_NotRecognized is no stackmode can be read, otherwise the
 * stackmode bits.
 */
static ARMWord
GetStackMode (bool isLoad, bool doLowerCase)
{
  ARMWord stackMode = kOption_NotRecognized;
  const char c1 = Input_Look ();
  if (c1 == (doLowerCase ? 'd' : 'D'))
    {
      const char c2 = Input_LookN (1);
      if (c2 == (doLowerCase ? 'b' : 'B'))
	stackMode = STACKMODE_DB;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = STACKMODE_DA;
    }
  else if (c1 == (doLowerCase ? 'e' : 'E'))
    {
      const char c2 = Input_LookN (1);
      if (c2 == (doLowerCase ? 'd' : 'D'))
	stackMode = isLoad ? STACKMODE_IB : STACKMODE_DA;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = isLoad ? STACKMODE_DB : STACKMODE_IA;
    }
  else if (c1 == (doLowerCase ? 'f' : 'F'))
    {
      const char c2 = Input_LookN (1);
      if (c2 == (doLowerCase ? 'd' : 'D'))
	stackMode = isLoad ? STACKMODE_IA : STACKMODE_DB;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = isLoad ? STACKMODE_DA : STACKMODE_IB;
    }
  else if (c1 == (doLowerCase ? 'i' : 'I'))
    {
      const char c2 = Input_LookN (1);
      if (c2 == (doLowerCase ? 'b' : 'B'))
	stackMode = STACKMODE_IB;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = STACKMODE_IA;
    }

  if (stackMode != kOption_NotRecognized)
    Input_SkipN (2);

  return stackMode;
}


static ARMWord
GetFPAPrecision (bool forLdfStfUsage, bool doLowerCase)
{
  if (Input_Match (doLowerCase ? 's' : 'S', false))
    return forLdfStfUsage ? PRECISION_MEM_SINGLE : PRECISION_SINGLE;
  if (Input_Match (doLowerCase ? 'd' : 'D', false))
    return forLdfStfUsage ? PRECISION_MEM_DOUBLE : PRECISION_DOUBLE;
  if (Input_Match (doLowerCase ? 'e' : 'E', false))
    return forLdfStfUsage ? PRECISION_MEM_EXTENDED : PRECISION_EXTENDED;
  if (Input_Match (doLowerCase ? 'p' : 'P', false))
    return forLdfStfUsage ? PRECISION_MEM_PACKED : kOption_NotRecognized;
  return kOption_NotRecognized;
}


static ARMWord
GetFPARounding (bool doLowerCase)
{
  if (Input_Match (doLowerCase ? 'p' : 'P', false))
    return ROUND_PLUSINF;
  if (Input_Match (doLowerCase ? 'm' : 'M', false))
    return ROUND_MINUSINF;
  if (Input_Match (doLowerCase ? 'z' : 'Z', false))
    return ROUND_ZERO;
  return ROUND_NEAREST;
}


/**
 * Checks if the end of the current keyword has been reached or the
 * start of an instruction qualifier (i.e. the dot of ".N" or ".W")
 * The validness of the instruction qualifier (if there) is NOT checked.
 * \param option Value to return when the end of current keyword has been
 * reached.  Might already be kOption_NotRecognized as well.
 * \return kOption_NotRecognized when end of current keyword or start of
 * an instruciton qualifier has not been found, otherwise the given option
 * value.
 */
static ARMWord
IsEndOfKeyword (ARMWord option)
{
  return (Input_IsEndOfKeyword () || Input_Look () == '.') ? option : kOption_NotRecognized;
}


/**
 * Try to parse condition code at the end of a keyword.
 */
ARMWord
Option_Cond (bool doLowerCase)
{
  return IsEndOfKeyword (GetCCode (doLowerCase));
}


/**
 * For ADC, ADD, AND, BIC, EOR, MOV, MVN, ORR, RSB, RSC, SBC, SUB instructions.
 * Try to parse [<cc>]["S"] (pre-UAL) and/or ["S"][<cc>] (UAL) + end-of-keyword.
 */
ARMWord
Option_CondS (bool doLowerCase)
{
  return GetOptCCodeAndOptChar (doLowerCase, 'S', PSR_S_FLAG);
}


/**
 * Try to parse ["S"][<cc>] + end-of-keyword.
 * Strictly UAL syntax only.
 */
ARMWord
Option_SCond (bool doLowerCase)
{
  ARMWord option = Input_Match (doLowerCase ? 's' : 'S', false) ? PSR_S_FLAG : 0;
  return IsEndOfKeyword (option | GetCCode (doLowerCase));
}


/**
 * For CMN, CMP, TEQ and TST instructions.
 *   [<cc>]["S"]["P"] : pre-UAL
 *   [<cc>]["P"]["S"] : pre-UAL
 *   [<cc>] : UAL
 *   + end-of-keyword.
 */
ARMWord
Option_CondSP (bool doLowerCase)
{
  Syntax_e syntax = State_GetSyntax ();
  bool gotS = syntax == eSyntax_UALOnly;

  if (!gotS)
    gotS = Input_Match (doLowerCase ? 's' : 'S', false);
  ARMWord option = GetCCode (doLowerCase) | PSR_S_FLAG;
  if (!gotS)
    gotS = Input_Match (doLowerCase ? 's' : 'S', false);
  bool gotP = syntax != eSyntax_UALOnly
		&& Input_Match (doLowerCase ? 'p' : 'P', false);
  if (!gotS)
    gotS = Input_Match (doLowerCase ? 's' : 'S', false);

  if (gotS && option_pedantic)
    Error (ErrorInfo, "%c suffix on comparison instruction is DEPRECATED", doLowerCase ? 's' : 'S');
  if (gotP)
    option |= PSR_P_FLAG;

  ARMWord result = IsEndOfKeyword (option);
  if (result != kOption_NotRecognized
      && gotP && (gOptionAPCS & APCS_OPT_32BIT) != 0)
    Error (ErrorWarning, "TSTP/TEQP/CMNP/CMPP inadvisable in 32-bit PC configurations");
  return result;
}


/**
 * For SWP(B) instructions.
 * Try to parse [<cc>]["B"] (pre-UAL) and/or ["B"][<cc>] (UAL) + end-of-keyword.
 */
ARMWord
Option_CondB (bool doLowerCase)
{
  return GetOptCCodeAndOptChar (doLowerCase, 'B', B_FLAG);
}


static ARMWord
Option_LdrStrType (bool isStore, bool doLowerCase)
{
  ARMWord option;
  if (Input_Match (doLowerCase ? 's' : 'S', false))
    {
      if (isStore)
	option = kOption_NotRecognized; /* "STR<cc>S<...>" is not possible.  */
      else
	{
	  /* "LDR<cc>SB" or "LDR<cc>SH".  */
	  if (Input_Match (doLowerCase ? 'h' : 'H', false))
	    option = H_FLAG | 0x90 | S_FLAG | L_FLAG; /* "LDR<cc>SH".  */
	  else if (Input_Match (doLowerCase ? 'b' : 'B', false))
	    option = 0x90 | S_FLAG | L_FLAG; /* "LDR<cc>SB".  */
	  else
	    option = kOption_NotRecognized;
	}
    }
  else
    {
      /* "" | "T" | "B" | "BT" | "D" | "H" */
      if (Input_Match (doLowerCase ? 'd' : 'D', false))
	{ /* "D". Address mode 3.  */
	  option = 0x90 | S_FLAG;
	  if (isStore)
	    option |= H_FLAG;
	}
      else if (Input_Match (doLowerCase ? 'h' : 'H', false))
	{ /* "H". Address mode 3.  */
	  option = 0x90 | H_FLAG;
	  if (!isStore)
	    option |= L_FLAG;
	}
      else
	{
	  if (Input_Match (doLowerCase ? 'b' : 'B', false))
	    { /* "B", "BT". Address mode 2.  */
	      option = B_FLAG;
	    }
	  else
	    option = 0;

	  /* "", "T", "B", "BT". Address mode 2.  */
	  option |= 1<<26;
	  if (!isStore)
	    option |= L_FLAG;
	  if (Input_Match (doLowerCase ? 't' : 'T', false))
	    option |= W_FLAG;
	}
    }

  return option;
}

/**
 * Try to parse:
 *   Pre-UAL:
 *     Supports [<cc>] [ "" | "T" | "B" | "BT" | "D" | "H" | "SB" | "SH" ]
 *     in LDR and STR.
 *     Note STR<cc>SB and STR<cc>SH are not supported, use STR<cc>B and
 *     STR<cc>H instead.
 *   UAL:
 *     Supports [ "" | "T" | "B" | "BT" | "D" | "H" | "SB" | "SH" ] [<cc>]
 *     in LDR and STR.
 *   + end-of-keyword.
 */
ARMWord
Option_LdrStrCondAndType (bool isStore, bool doLowerCase)
{
  ARMWord option = Option_GetCCodeIfThere (doLowerCase);
  if (option == kOption_NotRecognized)
    {
      /* No condition code recognised, try to parse <type> + [ <cc> ]
	 instead.  */
      option = Option_LdrStrType (isStore, doLowerCase);
      if (option != kOption_NotRecognized)
	option |= GetCCode (doLowerCase);
    }
  else
    option |= Option_LdrStrType (isStore, doLowerCase);
  
  return IsEndOfKeyword (option);
}


/**
 * For RFE/SRS instructions.
 * Try to parse address mode used in RFE and SRS + end-of-keyword.
 * Note, no condition code are read (that's only possible with Thumb-2).
 */
ARMWord
Option_CondRfeSrs (bool isLoad, bool doLowerCase)
{
  ARMWord option = GetStackMode (isLoad, doLowerCase);
  /* When there is no stack mode specified for SRS and RFE, it is implicitely
     IA for *both* cases.  */
  return IsEndOfKeyword (option == kOption_NotRecognized ? STACKMODE_IA : option);
}


/**
 * Try to parse [<cc>]<stackmode> (pre-UAL) and/or [<stackmode>][<cc>] (UAL)
 * + end-of-keyword.
 */
ARMWord
Option_CondLdmStm (bool isLDM, bool doLowerCase)
{
  Syntax_e syntax = State_GetSyntax ();
  ARMWord ccode = syntax == eSyntax_UALOnly ? kOption_NotRecognized : Option_GetCCodeIfThere (doLowerCase);

  ARMWord stackMode;
  if (syntax != eSyntax_PreUALOnly)
    {
      stackMode = GetStackMode (isLDM, doLowerCase); 
      if (stackMode == kOption_NotRecognized)
	stackMode = STACKMODE_IA;

      /* Second (or first in case of UAL-only) chance to parse a valid CC code.  */
      if (ccode == kOption_NotRecognized)
	ccode = Option_GetCCodeIfThere (doLowerCase);
    }
  else
    {
      /* Pre-UAL only syntax requires an explicit "IA"/"ia" as stackmode.  */
      stackMode = Input_MatchString (doLowerCase ? "ia" : "IA") ? STACKMODE_IA : kOption_NotRecognized;
    }
  if (ccode == kOption_NotRecognized)
    ccode = AL;

  return IsEndOfKeyword (ccode | stackMode);
}


/**
 * For LFM/SFM instructions.
 * Try to parse [<cc>].  NO end-of-keyword check.
 */
ARMWord
Option_CondLfmSfm (bool doLowerCase)
{
  return GetCCode (doLowerCase);
}


/**
 * For all FPA (except LDF/STF/LFM/SFM) instructions.
 * Try to parse [<cc>]["S" | "D" | "E" | "P"]["" | "P" | "M" | "Z"]
 * + end-of-keyword.
 */
ARMWord
Option_CondPrecRound (bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (kOption_NotRecognized == (option |= GetFPAPrecision (false, doLowerCase)))
    return kOption_NotRecognized;
  return IsEndOfKeyword (option | GetFPARounding (doLowerCase));
}


/**
 * For LDF/STF instructions.
 * Try to parse [<cc>]["S" | "D" | "E" | "P"] + end-of-keyword.
 */
ARMWord
Option_CondPrec_P (bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (kOption_NotRecognized == (option |= GetFPAPrecision (true, doLowerCase)))
    return kOption_NotRecognized;
  return IsEndOfKeyword (option);
}


/**
 * For LDC/STC instructions.
 * Try to parse [<cc>]["L"] (pre-UAL) and/or ["L"][<cc>] (UAL) + end-of-keyword.
 */
ARMWord
Option_CondL (bool doLowerCase)
{
  return GetOptCCodeAndOptChar (doLowerCase, 'L', N_FLAG);
}


/**
 * For the FIX instruction.
 * Try to parse [<cc>]["" | "P" | "M" | "Z"] + end-of-keyword.
 */
ARMWord
Option_CondOptRound (bool doLowerCase)
{
  ARMWord Option_CC = GetCCode (doLowerCase);
  return IsEndOfKeyword (Option_CC | PRECISION_SINGLE | GetFPARounding (doLowerCase));
}


/**
 * For B/BL instructions.
 * Try to parse ["L"][<cc>] + end-of-keyword.
 */
ARMWord
Option_LinkCond (bool doLowerCase)
{
  ParseState state;
  ParseState_Init (&state, doLowerCase);
  bool ok;
  if (!(ok = ParseState_IsOptCC (&state) && ParseState_IsEndOfKeyword (&state)))
    {
      ParseState_Init (&state, doLowerCase);
      ok = ParseState_IsOptChar (&state, LINK_BIT, 'L')
	     && ParseState_IsOptCC (&state) && ParseState_IsEndOfKeyword (&state);
    }
  return ok ? state.cc : kOption_NotRecognized;
}


/**
 * For CMF(E)/CNF(E) FPE instructions.
 * Try to parse ["E"][<cc>] + end-of-keyword.
 */
ARMWord
Option_ExceptionCond (bool doLowerCase)
{
  ParseState state;
  ParseState_Init (&state, doLowerCase);
  bool ok;
  if (!(ok = ParseState_IsOptCC (&state) && ParseState_IsEndOfKeyword (&state)))
    {
      ParseState_Init (&state, doLowerCase);
      ok = ParseState_IsOptChar (&state, EXCEPTION_BIT, 'E')
	     && ParseState_IsOptCC (&state) && ParseState_IsEndOfKeyword (&state);
    }
  return ok ? state.cc : kOption_NotRecognized;
}


/**
 * For the ADR instruction and ADRL pseudo-instruction.
 * Try to parse [<cc>]["L"] (pre-UAL) and/or ["L"][<cc>] (UAL) + end-of-keyword.
 */
ARMWord
Option_ADRL (bool doLowerCase)
{
  return GetOptCCodeAndOptChar (doLowerCase, 'L', 1);
}


bool
Option_IsValidARMMode (unsigned armMode)
{
  return armMode == ARM_Mode_eUSR
	   || armMode == ARM_Mode_eFIQ
	   || armMode == ARM_Mode_eIRQ
	   || armMode == ARM_Mode_eSVC
	   || armMode == ARM_Mode_eMON
	   || armMode == ARM_Mode_eABT
	   || armMode == ARM_Mode_eHYP
	   || armMode == ARM_Mode_eUND
	   || armMode == ARM_Mode_eSYS;
}
