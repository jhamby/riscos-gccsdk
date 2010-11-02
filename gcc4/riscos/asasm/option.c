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
isOK (ARMWord option)
{
  if (inputLook () && !isspace ((unsigned char)inputGet ()))
    return optionError;
  else
    return option;
}


ARMWord
optionCond (void)
{
  return isOK (getCond ());
}


ARMWord
optionCondS (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('S', false))
    option |= S_FLAG;
  return isOK (option);
}


ARMWord
optionCondSP (void)
{
  ARMWord option = getCond () | S_FLAG;
  if (Input_Match ('S', false) && option_pedantic)
    error (ErrorInfo, "S is implicit in test instructions");
  if (Input_Match ('P', false))
    option |= P_FLAG;
  return isOK (option);
}


ARMWord
optionCondB (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('B', false))
    option |= B_FLAG;
  return isOK (option);
}


/* also does signed byte, (un)signed halfword and doubleword */
ARMWord
optionCondBT (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('S', false))
    {
      option |= 0xD0;
      switch (inputLook ())
	{
	case 'H':
	  option |= 0x20;	/* fall through to 'b' */
	case 'B':
	  inputSkip ();
	  break;
	default:
	  option = optionError;
	  break;
	}
    }
  else
    {
      switch (inputLook ())
	{
	case 'D':
	  /* Use bit 27 as a flag for doubleword access */
	  option |= 0xD0 | (1 << 27);
	  inputSkip ();
	  break;
	case 'H':
	  option |= 0xB0;
	  inputSkip ();
	  break;
	case 'B':
	  option |= B_FLAG;
	  inputSkip ();
	  break;
	}
    }
  if (Input_Match ('T', false))
    option |= T_FLAG;
  return isOK (option);
}


ARMWord
optionCondDirLdm (void)
{
  ARMWord option = getCond ();
  if (optionError == (option |= getDir (true)))
    return optionError;
  return isOK (option);
}


ARMWord
optionCondDirStm (void)
{
  ARMWord option = getCond ();
  if (optionError == (option |= getDir (false)))
    return optionError;
  return isOK (option);
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
  return isOK (option | getRound ());
}


ARMWord
optionCondOptPrecRound (void)
{
  ARMWord optionCC = getCond ();
  ARMWord optionPrec = getPrec (false);
  if (optionError == optionPrec)
    optionPrec = PRECISION_EXTENDED;
  return isOK (optionCC | optionPrec | getRound ());
}


ARMWord
optionCondPrec_P (void)
{
  ARMWord option = getCond ();
  if (optionError == (option |= getPrec (true)))
    return optionError;
  return isOK (option);
}


ARMWord
optionCondL (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('L', false))
    option |= L_FLAG;
  return isOK (option);
}


/****** Trouble mnemonics **********/



ARMWord
optionCondOptRound (void)
{
  ARMWord optionCC = getCond ();
  return isOK (optionCC | PRECISION_SINGLE | getRound ());
}


/* 'B' is matched before call */
ARMWord
optionLinkCond (void)
{
  if (inputLook () != 'L')
    return isOK (getCond ()); /* Only b.CC possible  */

  inputSkip ();		/* bl.CC or b.l?  */
  switch (inputLook ())
    {
      case 'E':		/* b.le or bl.eq */
	inputSkip ();
	switch (inputLook ())
	  {
	    case 'Q':		/* Only bl.eq */
	      inputSkip ();
	      return isOK (EQ | LINK_BIT);
	    default:		/* Only b.le */
	      return isOK (LE);
	  }
      case 'O':		/* Only b.lo possible */
	inputSkip ();
	return isOK (LO);
      case 'S':		/* Only b.ls possible */
	inputSkip ();
	return isOK (LS);
      case 'T':		/* Only b.lt possible */
	inputSkip ();
	return isOK (LT);
      default:		/* Only bl.CC possible */
	return isOK (getCond () | LINK_BIT);
    }
  return optionError;
}


ARMWord
optionExceptionCond (void)
{
  if (!Input_Match ('E', false))
    return isOK (getCond ()); /* Only cmf.CC possible  */
  /* cmf.eq or cmfe.CC */
  if (Input_Match ('Q', false))
    return isOK (EQ); /* Only cmf.eq */
  /* Only cmfe.CC */
  return isOK (getCond () | EXEPTION_BIT);
}


ARMWord
optionAdrL (void)
{
  ARMWord option = getCond ();
  if (Input_Match ('L', false))
    option |= 1;
  return isOK (option);
}
