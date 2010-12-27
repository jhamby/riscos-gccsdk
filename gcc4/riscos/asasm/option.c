/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developersrs
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

static ARMWord
getCond (void)
{
  ARMWord cc = AL;
  switch (inputLook ())
    {
    case 'A':
      switch (inputLookN (1))
	{
	case 'L':
	  cc = AL;
	  inputSkipN (2);
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
  if (cc != AL)
    inputSkipN (2);
  return cc;
}


static ARMWord
getDir (bool ldm)
{
  ARMWord dir = optionError;
  switch (inputLook ())
    {
    case 'D':
      switch (inputLookN (1))
	{
	case 'B':
	  dir = DB;
	  inputSkipN (2);
	  break;
	case 'A':
	  dir = DA;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'E':
      switch (inputLookN (1))
	{
	case 'D':
	  dir = (ldm ? IB : DA);
	  inputSkipN (2);
	  break;
	case 'A':
	  dir = (ldm ? DB : IA);
	  inputSkipN (2);
	  break;
	}
      break;
    case 'F':
      switch (inputLookN (1))
	{
	case 'D':
	  dir = (ldm ? IA : DB);
	  inputSkipN (2);
	  break;
	case 'A':
	  dir = (ldm ? DA : IB);
	  inputSkipN (2);
	  break;
	}
      break;
    case 'I':
      switch (inputLookN (1))
	{
	case 'B':
	  dir = IB;
	  inputSkipN (2);
	  break;
	case 'A':
	  dir = IA;
	  inputSkipN (2);
	  break;
	}
      break;
    }
  return dir;
}


static ARMWord
getPrec (int P)
{
  switch (inputGet ())
    {
      case 'S':
        return P ? PRECISION_MEM_SINGLE : PRECISION_SINGLE;
      case 'D':
        return P ? PRECISION_MEM_DOUBLE : PRECISION_DOUBLE;
      case 'E':
        return P ? PRECISION_MEM_EXTENDED : PRECISION_EXTENDED;
      case 'P':
        return P ? PRECISION_MEM_PACKED : optionError;
    }
  return optionError;
}


static ARMWord
getRound (void)
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


static ARMWord
IsEndOfKeyword (ARMWord option)
{
  unsigned char c = (unsigned char)inputLook ();
  return (c != '\0' && !isspace (c) && c != ';') ? optionError : option;
}


ARMWord
optionCond (void)
{
  return IsEndOfKeyword (getCond ());
}


ARMWord
optionCondS (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('S', false))
    option |= PSR_S_FLAG;
  return IsEndOfKeyword (option);
}


/**
 * Tries to parse "S" (optionally) followed by condition code.
 */
ARMWord
Option_SCond (void)
{
  ARMWord option = Input_Match ('S', false) ? PSR_S_FLAG : 0;
  return IsEndOfKeyword (option | getCond ());
}


/**
 * Used for CMN, CMP, TEQ and TST.
 */
ARMWord
optionCondSP (void)
{
  ARMWord option = getCond () | PSR_S_FLAG;
  if (Input_Match ('S', false) && option_pedantic)
    error (ErrorInfo, "S is implicit in test instructions");
  if (Input_Match ('P', false))
    {
      option |= PSR_P_FLAG;
      if (option_apcs_32bit)
	error (ErrorWarning, "TSTP/TEQP/CMNP/CMPP inadvisable in 32-bit PC configurations");
    }
  return IsEndOfKeyword (option);
}


ARMWord
optionCondB (void)
{
  ARMWord option = getCond ();
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
  ARMWord option = getCond ();
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


ARMWord
optionCondLdmStm (bool isLDM)
{
  ARMWord option = getCond ();
  if (optionError == (option |= getDir (isLDM)))
    return optionError;
  return IsEndOfKeyword (option);
}


ARMWord
optionCondLfmSfm (void)
{
  return getCond ();
}


ARMWord
optionCondPrecRound (void)
{
  ARMWord option = getCond ();
  if (optionError == (option |= getPrec (false)))
    return optionError;
  return IsEndOfKeyword (option | getRound ());
}


ARMWord
optionCondOptPrecRound (void)
{
  ARMWord optionCC = getCond ();
  ARMWord optionPrec = getPrec (false);
  if (optionError == optionPrec)
    optionPrec = PRECISION_EXTENDED;
  return IsEndOfKeyword (optionCC | optionPrec | getRound ());
}


ARMWord
optionCondPrec_P (void)
{
  ARMWord option = getCond ();
  if (optionError == (option |= getPrec (true)))
    return optionError;
  return IsEndOfKeyword (option);
}


ARMWord
optionCondL (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('L', false))
    option |= N_FLAG;
  return IsEndOfKeyword (option);
}


/****** Trouble mnemonics **********/



ARMWord
optionCondOptRound (void)
{
  ARMWord optionCC = getCond ();
  return IsEndOfKeyword (optionCC | PRECISION_SINGLE | getRound ());
}


/* 'B' is matched before call */
ARMWord
optionLinkCond (void)
{
  if (inputLook () != 'L')
    return IsEndOfKeyword (getCond ()); /* Only b.CC possible  */

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
	return IsEndOfKeyword (getCond () | LINK_BIT);
    }
  return optionError;
}


ARMWord
optionExceptionCond (void)
{
  if (!Input_Match ('E', false))
    return IsEndOfKeyword (getCond ()); /* Only cmf.CC possible  */
  /* cmf.eq or cmfe.CC */
  if (Input_Match ('Q', false))
    return IsEndOfKeyword (EQ); /* Only cmf.eq */
  /* Only cmfe.CC */
  return IsEndOfKeyword (getCond () | EXEPTION_BIT);
}


ARMWord
optionAdrL (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('L', false))
    option |= 1;
  return IsEndOfKeyword (option);
}
