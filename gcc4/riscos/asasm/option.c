/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2011 GCCSDK Developersrs
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

/**
 * Try to parse a 2 character condition code.
 */
static ARMWord
GetCCode (void)
{
  ARMWord cc = optionError;
  switch (inputLook ())
    {
      case 'A':
	switch (inputLookN (1))
	  {
	    case 'L':
	      cc = AL;
	      break;
	  }
	break;
      case 'C':
	switch (inputLookN (1))
	  {
	    case 'C':
	      cc = CC;
	      break;
	    case 'S':
	      cc = CS;
	      break;
	  }
	break;
      case 'E':
	switch (inputLookN (1))
	  {
	    case 'Q':
	      cc = EQ;
	      break;
	  }
	break;
      case 'G':
	switch (inputLookN (1))
	  {
	    case 'E':
	      cc = GE;
	      break;
	    case 'T':
	      cc = GT;
	      break;
	  }
	break;
      case 'H':
	switch (inputLookN (1))
	  {
	    case 'I':
	      cc = HI;
	      break;
	    case 'S':
	      cc = HS;
	      break;
	  }
	break;
      case 'L':
	switch (inputLookN (1))
	  {
	    case 'E':
	      cc = LE;
	      break;
	    case 'O':
	      cc = LO;
	      break;
	    case 'S':
	      cc = LS;
	      break;
	    case 'T':
	      cc = LT;
	      break;
	  }
	break;
      case 'M':
	switch (inputLookN (1))
	  {
	    case 'I':
	      cc = MI;
	      break;
	  }
	break;
      case 'N':
	switch (inputLookN (1))
	  {
	    case 'E':
	      cc = NE;
	      break;
	    case 'V':
	      cc = NV;
	      break;
	  }
	break;
      case 'P':
	switch (inputLookN (1))
	  {
	    case 'L':
	      cc = PL;
	      break;
	  }
	break;
      case 'V':
	switch (inputLookN (1))
	  {
	    case 'C':
	      cc = VC;
	      break;
	    case 'S':
	      cc = VS;
	      break;
	  }
	break;
    }
  if (cc != optionError)
    inputSkipN (2);
  else
    cc = AL;
  return cc;
}


/**
 * Tried to read stackmode for LDM/STM/RFE/SRS.
 * \return optionError is no stackmode can be read, otherwise the stackmode
 * bits.
 */
static ARMWord
GetStackMode (bool isLoad)
{
  ARMWord stackMode = optionError;
  switch (inputLook ())
    {
      case 'D':
	switch (inputLookN (1))
	  {
	    case 'B':
	      stackMode = STACKMODE_DB;
	      break;
	    case 'A':
	      stackMode = STACKMODE_DA;
	      break;
	  }
	break;
      case 'E':
	switch (inputLookN (1))
	  {
	    case 'D':
	      stackMode = isLoad ? STACKMODE_IB : STACKMODE_DA;
	      break;
	    case 'A':
	      stackMode = isLoad ? STACKMODE_DB : STACKMODE_IA;
	      break;
	  }
	break;
      case 'F':
	switch (inputLookN (1))
	  {
	    case 'D':
	      stackMode = isLoad ? STACKMODE_IA : STACKMODE_DB;
	      break;
	    case 'A':
	      stackMode = isLoad ? STACKMODE_DA : STACKMODE_IB;
	      break;
	  }
	break;
      case 'I':
	switch (inputLookN (1))
	  {
	    case 'B':
	      stackMode = STACKMODE_IB;
	      break;
	    case 'A':
	      stackMode = STACKMODE_IA;
	      break;
	  }
	break;
    }
  if (stackMode != optionError)
    inputSkipN (2);

  return stackMode;
}


static ARMWord
GetFPAPrecision (bool forLdfStfUsage)
{
  switch (inputGet ())
    {
      case 'S':
	return forLdfStfUsage ? PRECISION_MEM_SINGLE : PRECISION_SINGLE;
      case 'D':
	return forLdfStfUsage ? PRECISION_MEM_DOUBLE : PRECISION_DOUBLE;
      case 'E':
	return forLdfStfUsage ? PRECISION_MEM_EXTENDED : PRECISION_EXTENDED;
      case 'P':
	return forLdfStfUsage ? PRECISION_MEM_PACKED : optionError;
    }
  return optionError;
}


static ARMWord
GetFPARounding (void)
{
  switch (inputLook ())
    {
      case 'P':
	inputSkip ();
	return ROUND_PLUSINF;
      case 'M':
	inputSkip ();
	return ROUND_MINUSINF;
      case 'Z':
	inputSkip ();
	return ROUND_ZERO;
    }
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
  return Input_IsEndOfKeyword () ? option : optionError;
}


ARMWord
optionCond (void)
{
  return IsEndOfKeyword (GetCCode ());
}


/**
 * Tries to parse condition code and "S" (both optionally and in any order)
 * and that should terminate the keyword.
 * I.e. support pre-UAL and UAL syntax.
 */
ARMWord
optionCondS (void)
{
  ARMWord option;
  if (Input_Match ('S', false))
    option = PSR_S_FLAG | GetCCode ();
  else
    {
      option = GetCCode ();
      if (Input_Match ('S', false))
	option |= PSR_S_FLAG;
    }
  return IsEndOfKeyword (option);
}


/**
 * Tries to parse "S" (optionally) followed by condition code (i.e. strictly
 * UAL syntax only) and that should terminate the keyword.
 */
ARMWord
Option_SCond (void)
{
  ARMWord option = Input_Match ('S', false) ? PSR_S_FLAG : 0;
  return IsEndOfKeyword (option | GetCCode ());
}


/**
 * Used for CMN, CMP, TEQ and TST.
 */
ARMWord
optionCondSP (void)
{
  ARMWord option = GetCCode () | PSR_S_FLAG;
  if (Input_Match ('S', false) && option_pedantic)
    error (ErrorInfo, "S is implicit in test instructions");
  if (Input_Match ('P', false))
    {
      option |= PSR_P_FLAG;
      if (gOptionAPCS & APCS_OPT_32BIT)
	error (ErrorWarning, "TSTP/TEQP/CMNP/CMPP inadvisable in 32-bit PC configurations");
    }
  return IsEndOfKeyword (option);
}


ARMWord
optionCondB (void)
{
  ARMWord option = GetCCode ();
  if (Input_Match ('B', false))
    option |= B_FLAG;
  return IsEndOfKeyword (option);
}


/**
 * Supports {<cond>} [ "" | "T" | "B" | "BT" | "D" | "H" | "SB" | "SH" ]
 * in LDR and STR.
 * Note STR<cond>SB and STR<cond>SH are not supported, use STR<cond>B and
 * STR<cond>H instead.
 */
ARMWord
optionCondBT (bool isStore)
{
  ARMWord option = GetCCode ();
  if (Input_Match ('S', false))
    {
      if (isStore)
	option = optionError;
      else
	{
	  /* "LDR<cond>SB" or "LDR<cond>SH".  */
	  switch (inputLook ())
	    {
	      case 'H': /* "LDR<cond>SH".  */
		option |= H_FLAG;
	      /* Fall through.  */

	      case 'B': /* "LDR<cond>SB".  */
		option |= 0x90 | S_FLAG | L_FLAG;
		inputSkip ();
	      break;

	      default:
		option = optionError;
		break;
	    }
	}
    }
  else
    {
      /* "" | "T" | "B" | "BT" | "D" | "H" */
      switch (inputLook ())
	{
	  case 'D': /* "D". Address mode 3.  */
	    option |= 0x90 | S_FLAG;
	    if (isStore)
	      option |= H_FLAG;
	    inputSkip ();
	    break;

	  case 'H': /* "H". Address mode 3.  */
	    option |= 0x90 | H_FLAG;
	    if (!isStore)
	      option |= L_FLAG;
	    inputSkip ();
	    break;

	  case 'B': /* "B", "BT". Address mode 2.  */
	    option |= B_FLAG;
	    inputSkip ();
	    /* Fall through.  */

	  default: /* "", "T", "B", "BT". Address mode 2.  */
	    option |= 1<<26;
	    if (!isStore)
	      option |= L_FLAG;
	    if (Input_Match ('T', false))
	      option |= W_FLAG;
	    break;
	}
    }

  return IsEndOfKeyword (option);
}


/**
 * Tries to parse address mode used in RFE and SRS.
 * Note, no condition code are read (that's only possible with Thumb-2).
 */
ARMWord
Option_CondRfeSrs (bool isLoad)
{
  ARMWord option = GetStackMode (isLoad);
  /* When there is no stack mode specified for SRS and RFE, it is implicitely
     IA for *both* cases.  */
  return IsEndOfKeyword (option == optionError ? STACKMODE_IA : option);
}


ARMWord
optionCondLdmStm (bool isLDM)
{
  ARMWord option = GetCCode ();
  if (option == optionError)
    return optionError;
  ARMWord stackMode = GetStackMode (isLDM);
  if (stackMode == optionError)
    stackMode = (isLDM) ? STACKMODE_IA : STACKMODE_DB;
  return IsEndOfKeyword (option | stackMode);
}


ARMWord
optionCondLfmSfm (void)
{
  return GetCCode ();
}


/**
 * For all FPA (except LDF/STF) implementations.
 */
ARMWord
optionCondPrecRound (void)
{
  ARMWord option = GetCCode ();
  if (optionError == (option |= GetFPAPrecision (false)))
    return optionError;
  return IsEndOfKeyword (option | GetFPARounding ());
}


/**
 * For LDF/STF implementation.
 */
ARMWord
optionCondPrec_P (void)
{
  ARMWord option = GetCCode ();
  if (optionError == (option |= GetFPAPrecision (true)))
    return optionError;
  return IsEndOfKeyword (option);
}


ARMWord
optionCondL (void)
{
  ARMWord option = GetCCode ();
  if (Input_Match ('L', false))
    option |= N_FLAG;
  return IsEndOfKeyword (option);
}


/****** Trouble mnemonics **********/



ARMWord
optionCondOptRound (void)
{
  ARMWord optionCC = GetCCode ();
  return IsEndOfKeyword (optionCC | PRECISION_SINGLE | GetFPARounding ());
}


/* 'B' is matched before call */
ARMWord
optionLinkCond (void)
{
  if (inputLook () != 'L')
    return IsEndOfKeyword (GetCCode ()); /* Only b.CC possible  */

  inputSkip ();		/* bl.CC or b.l?  */
  switch (inputLook ())
    {
      case 'E':		/* b.le or bl.eq */
	inputSkip ();
	switch (inputLook ())
	  {
	    case 'Q':		/* Only bl.eq */
	      inputSkip ();
	      return IsEndOfKeyword (EQ | LINK_BIT);
	    default:		/* Only b.le */
	      return IsEndOfKeyword (LE);
	  }
      case 'O':		/* Only b.lo possible */
	inputSkip ();
	return IsEndOfKeyword (LO);
      case 'S':		/* Only b.ls possible */
	inputSkip ();
	return IsEndOfKeyword (LS);
      case 'T':		/* Only b.lt possible */
	inputSkip ();
	return IsEndOfKeyword (LT);
      default:		/* Only bl.CC possible */
	return IsEndOfKeyword (GetCCode () | LINK_BIT);
    }
  return optionError;
}


ARMWord
optionExceptionCond (void)
{
  if (!Input_Match ('E', false))
    return IsEndOfKeyword (GetCCode ()); /* Only cmf.CC possible  */
  /* cmf.eq or cmfe.CC */
  if (Input_Match ('Q', false))
    return IsEndOfKeyword (EQ); /* Only cmf.eq */
  /* Only cmfe.CC */
  return IsEndOfKeyword (GetCCode () | EXEPTION_BIT);
}


ARMWord
optionAdrL (void)
{
  ARMWord option = GetCCode ();
  if (Input_Match ('L', false))
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
