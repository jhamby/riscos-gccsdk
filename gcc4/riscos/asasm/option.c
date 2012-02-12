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
 * option.c
 */

#include "config.h"
#include <ctype.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "error.h"
#include "input.h"
#include "main.h"
#include "option.h"
#include "targetcpu.h"

/**
 * Try to parse a 2 character condition code.
 * \return Parsed condition code.  When there is no condition code,
 * kOption_NotRecognized is returned instead.
 */
static ARMWord
GetCCodeIfThere (bool doLowerCase)
{
  ARMWord cc = kOption_NotRecognized;
  const char c1 = inputLook ();
  if (c1 == (doLowerCase ? 'a' : 'A'))
    {
      if (inputLookN (1) == (doLowerCase ? 'l' : 'L'))
	cc = AL;
    }
  else if (c1 == (doLowerCase ? 'c' : 'C'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'c' : 'C'))
	cc = CC;
      else if (c2 == (doLowerCase ? 's' : 'S'))
	cc = CS;
    }
  else if (c1 == (doLowerCase ? 'e' : 'E'))
    {
      if (inputLookN (1) == (doLowerCase ? 'q' : 'Q'))
	cc = EQ;
    }
  else if (c1 == (doLowerCase ? 'g' : 'G'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'e' : 'E'))
	cc = GE;
      else if (c2 == (doLowerCase ? 't' : 'T'))
	cc = GT;
    }
  else if (c1 == (doLowerCase ? 'h' : 'H'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'i' : 'I'))
	cc = HI;
      else if (c2 == (doLowerCase ? 's' : 'S'))
	cc = CS; /* HS == CS */
    }
  else if (c1 == (doLowerCase ? 'l' : 'L'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'e' : 'E'))
	cc = LE;
      else if (c2 == (doLowerCase ? 'o' : 'O'))
	cc = CC; /* LO == CC */
      else if (c2 == (doLowerCase ? 's' : 'S'))
	cc = LS;
      else if (c2 == (doLowerCase ? 't' : 'T'))
	cc = LT;
    }
  else if (c1 == (doLowerCase ? 'm' : 'M'))
    {
      if (inputLookN (1) == (doLowerCase ? 'i' : 'I'))
	cc = MI;
    }
  else if (c1 == (doLowerCase ? 'n' : 'N'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'e' : 'E'))
	cc = NE;
      else if (c2 == (doLowerCase ? 'v' : 'V'))
	{
	  /* Pre-ARMv4, NV is supported.
	     ARMv4, NV is UNPREDICTABLE.
	     ARMv5 and later, used for "Unconditional instruction extension space"  */
	  ARM_eArchitectures arch = Target_GetArch ();
	  if (arch >= ARCH_ARMv5)
	    {
	      /* cc already is kOption_NotRecognized.  */
	    }
	  else if (arch >= ARCH_ARMv4)
	    {
	      error (ErrorWarning, "For ARMv4, use of NV condition code is UNPREDICTABLE");
	      cc = NV;
	    }
	  else
	    cc = NV;
	}
    }
  else if (c1 == (doLowerCase ? 'p' : 'P'))
    {
      if (inputLookN (1) == (doLowerCase ? 'l' : 'L'))
	cc = PL;
    }
  else if (c1 == (doLowerCase ? 'v' : 'V'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'c' : 'C'))
	cc = VC;
      else if (c2 == (doLowerCase ? 's' : 'S'))
	cc = VS;
    }

  if (cc != kOption_NotRecognized)
    inputSkipN (2);
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
  ARMWord ccode = GetCCodeIfThere (doLowerCase);
  if (ccode == kOption_NotRecognized)
    ccode = AL;
  return ccode;
}


/**
 * Try to read the instruction width indicator (".W"/".w" or ".N"/".n").
 * \return doLowerCase When true, instruction width indication should be
 * lowercase, uppercase otherwise.
 */
InstrWidth_e
Option_GetInstrWidth (bool doLowerCase)
{
  if (Input_MatchString (doLowerCase ? ".w" : ".W") && Input_IsEndOfKeyword ())
    return eInstrWidth_Enforce32bit;
  if (Input_MatchString (doLowerCase ? ".n" : ".N") && Input_IsEndOfKeyword ())
    return eInstrWidth_Enforce16bit;
  return eInstrWidth_NotSpecified;
}


/**
 * Try to read stackmode for LDM/STM/RFE/SRS.
 * \return kOption_NotRecognized is no stackmode can be read, otherwise the stackmode
 * bits.
 */
static ARMWord
GetStackMode (bool isLoad, bool doLowerCase)
{
  ARMWord stackMode = kOption_NotRecognized;
  const char c1 = inputLook ();
  if (c1 == (doLowerCase ? 'd' : 'D'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'b' : 'B'))
	stackMode = STACKMODE_DB;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = STACKMODE_DA;
    }
  else if (c1 == (doLowerCase ? 'e' : 'E'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'd' : 'D'))
	stackMode = isLoad ? STACKMODE_IB : STACKMODE_DA;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = isLoad ? STACKMODE_DB : STACKMODE_IA;
    }
  else if (c1 == (doLowerCase ? 'f' : 'F'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'd' : 'D'))
	stackMode = isLoad ? STACKMODE_IA : STACKMODE_DB;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = isLoad ? STACKMODE_DA : STACKMODE_IB;
    }
  else if (c1 == (doLowerCase ? 'i' : 'I'))
    {
      const char c2 = inputLookN (1);
      if (c2 == (doLowerCase ? 'b' : 'B'))
	stackMode = STACKMODE_IB;
      else if (c2 == (doLowerCase ? 'a' : 'A'))
	stackMode = STACKMODE_IA;
    }

  if (stackMode != kOption_NotRecognized)
    inputSkipN (2);

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
 * Checks if the end of the current keyword has been reached.
 * \param option Value to return when the end of current keyword has been
 * reached.
 * \return -1 when end of current keyword has not been reached, otherwise
 * the given option value.
 */
static ARMWord
IsEndOfKeyword (ARMWord option)
{
  if (option == kOption_NotRecognized)
    return option;
  return Input_IsEndOfKeyword () || Input_Match ('.', false) ? option : kOption_NotRecognized;
}


/**
 * Try to parse condition code.
 */
ARMWord
optionCond (bool doLowerCase)
{
  return IsEndOfKeyword (GetCCode (doLowerCase));
}


/**
 * Try to parse condition code and "S" (both optionally and in any order)
 * and that should terminate the keyword.
 * I.e. support pre-UAL and UAL syntax.
 */
ARMWord
optionCondS (bool doLowerCase)
{
  ARMWord option;
  if (Input_Match (doLowerCase ? 's' : 'S', false))
    option = PSR_S_FLAG | GetCCode (doLowerCase);
  else
    {
      option = GetCCode (doLowerCase);
      if (Input_Match (doLowerCase ? 's' : 'S', false))
	option |= PSR_S_FLAG;
    }
  return IsEndOfKeyword (option);
}


/**
 * Tries to parse "S" (optionally) followed by condition code (i.e. strictly
 * UAL syntax only) and that should terminate the keyword.
 */
ARMWord
Option_SCond (bool doLowerCase)
{
  ARMWord option = Input_Match (doLowerCase ? 's' : 'S', false) ? PSR_S_FLAG : 0;
  return IsEndOfKeyword (option | GetCCode (doLowerCase));
}


/**
 * Used for CMN, CMP, TEQ and TST.
 *   <condition code> "S" ["P"] : pre-UAL
 *   <condition code> "P" ["S"] : pre-UAL
 *   "S" <condition code> : UAL
 * Support pre-UAL and UAL syntax (order of (deprecated) "S" and condition
 * code is irrelevant).
 */
ARMWord
Option_CondSP (bool doLowerCase)
{
  bool gotS = Input_Match (doLowerCase ? 's' : 'S', false);
  ARMWord option = GetCCode (doLowerCase) | PSR_S_FLAG;
  if (!gotS)
    gotS = Input_Match (doLowerCase ? 's' : 'S', false);
  bool gotP = Input_Match (doLowerCase ? 'p' : 'P', false);
  if (!gotS)
    gotS = Input_Match (doLowerCase ? 's' : 'S', false);

  if (gotS && option_pedantic)
    error (ErrorInfo, "%c suffix on comparison instruction is DEPRECATED", doLowerCase ? 's' : 'S');
  if (gotP)
    {
      option |= PSR_P_FLAG;
      if (gOptionAPCS & APCS_OPT_32BIT)
	error (ErrorWarning, "TSTP/TEQP/CMNP/CMPP inadvisable in 32-bit PC configurations");
    }

  return IsEndOfKeyword (option);
}


ARMWord
optionCondB (bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (Input_Match (doLowerCase ? 'b' : 'B', false))
    option |= B_FLAG;
  return IsEndOfKeyword (option);
}


static ARMWord
Option_LdrStrType (bool isStore, bool doLowerCase)
{
  ARMWord option;
  if (Input_Match (doLowerCase ? 's' : 'S', false))
    {
      if (isStore)
	option = kOption_NotRecognized; /* "STR<cond>S<...>" is not possible.  */
      else
	{
	  /* "LDR<cond>SB" or "LDR<cond>SH".  */
	  if (Input_Match (doLowerCase ? 'h' : 'H', false))
	    option = H_FLAG | 0x90 | S_FLAG | L_FLAG; /* "LDR<cond>SH".  */
	  else if (Input_Match (doLowerCase ? 'b' : 'B', false))
	    option = 0x90 | S_FLAG | L_FLAG; /* "LDR<cond>SB".  */
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
 * Pre-UAL:
 *   Supports {<cond>} [ "" | "T" | "B" | "BT" | "D" | "H" | "SB" | "SH" ]
 *   in LDR and STR.
 *   Note STR<cond>SB and STR<cond>SH are not supported, use STR<cond>B and
 *   STR<cond>H instead.
 * UAL:
 *   Supports [ "" | "T" | "B" | "BT" | "D" | "H" | "SB" | "SH" ] {<cond>}
 *   in LDR and STR.
 */
ARMWord
Option_LdrStrCondAndType (bool isStore, bool doLowerCase)
{
  ARMWord option = GetCCodeIfThere (doLowerCase);
  if (option == kOption_NotRecognized)
    {
      /* No condition code recognised, try to parse <type> + [ <cond> ]
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
 * Tries to parse address mode used in RFE and SRS.
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


ARMWord
optionCondLdmStm (bool isLDM, bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (option == kOption_NotRecognized)
    return kOption_NotRecognized;
  ARMWord stackMode = GetStackMode (isLDM, doLowerCase);
  if (stackMode == kOption_NotRecognized)
    stackMode = STACKMODE_IA;
  return IsEndOfKeyword (option | stackMode);
}


ARMWord
optionCondLfmSfm (bool doLowerCase)
{
  return GetCCode (doLowerCase);
}


/**
 * For all FPA (except LDF/STF) implementations.
 */
ARMWord
optionCondPrecRound (bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (kOption_NotRecognized == (option |= GetFPAPrecision (false, doLowerCase)))
    return kOption_NotRecognized;
  return IsEndOfKeyword (option | GetFPARounding (doLowerCase));
}


/**
 * For LDF/STF implementation.
 */
ARMWord
optionCondPrec_P (bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (kOption_NotRecognized == (option |= GetFPAPrecision (true, doLowerCase)))
    return kOption_NotRecognized;
  return IsEndOfKeyword (option);
}


ARMWord
optionCondL (bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (Input_Match (doLowerCase ? 'l' : 'L', false))
    option |= N_FLAG;
  return IsEndOfKeyword (option);
}


/****** Trouble mnemonics **********/



ARMWord
optionCondOptRound (bool doLowerCase)
{
  ARMWord optionCC = GetCCode (doLowerCase);
  return IsEndOfKeyword (optionCC | PRECISION_SINGLE | GetFPARounding (doLowerCase));
}


/* 'B' is matched before call */
ARMWord
optionLinkCond (bool doLowerCase)
{
  /* bl.CC or b.l ?  */
  if (!Input_Match (doLowerCase ? 'l' : 'L', false))
    return IsEndOfKeyword (GetCCode (doLowerCase)); /* Only b.CC possible  */
	
  if (Input_Match (doLowerCase ? 'e' : 'E', false))
    {
      /* b.le or bl.eq */
      if (Input_Match (doLowerCase ? 'q' : 'Q', false))
	return IsEndOfKeyword (EQ | LINK_BIT);
      return IsEndOfKeyword (LE);
    }
  if (Input_Match (doLowerCase ? 'o' : 'O', false))
    return IsEndOfKeyword (CC); /* Only b.lo possible.  LO == CC */
  if (Input_Match (doLowerCase ? 's' : 'S', false))
    return IsEndOfKeyword (LS); /* Only b.ls possible */
  if (Input_Match (doLowerCase ? 't' : 'T', false))
    return IsEndOfKeyword (LT); /* Only b.lt possible */
  /* Only bl.CC possible */
  return IsEndOfKeyword (GetCCode (doLowerCase) | LINK_BIT);
}


ARMWord
optionExceptionCond (bool doLowerCase)
{
  if (!Input_Match (doLowerCase ? 'e' : 'E', false))
    return IsEndOfKeyword (GetCCode (doLowerCase)); /* Only cmf.CC possible  */
  /* cmf.eq or cmfe.CC */
  if (Input_Match (doLowerCase ? 'q' : 'Q', false))
    return IsEndOfKeyword (EQ); /* Only cmf.eq */
  /* Only cmfe.CC */
  return IsEndOfKeyword (GetCCode (doLowerCase) | EXCEPTION_BIT);
}


ARMWord
optionAdrL (bool doLowerCase)
{
  ARMWord option = GetCCode (doLowerCase);
  if (Input_Match (doLowerCase ? 'l' : 'L', false))
    option |= 1;
  return IsEndOfKeyword (option);
}


bool
Option_IsValidARMMode (int armMode)
{
  return armMode == ARM_MODE_USR
	   || armMode == ARM_MODE_FIQ
	   || armMode == ARM_MODE_IRQ
	   || armMode == ARM_MODE_SVC
	   || armMode == ARM_MODE_ABORT
	   || armMode == ARM_MODE_UNDEF
	   || armMode == ARM_MODE_SYSTEM;
}
