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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "error.h"
#include "input.h"
#include "main.h"
#include "option.h"

static WORD
getCond (void)
{
  WORD cc = AL;
  switch (inputLookUC ())
    {
    case 'a':
      switch (inputLookNUC (1))
	{
	case 'l':
	  cc = AL;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'c':
      switch (inputLookNUC (1))
	{
	case 'c':
	  cc = CC;
	  inputSkipN (2);
	  break;
	case 's':
	  cc = CS;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'e':
      switch (inputLookNUC (1))
	{
	case 'q':
	  cc = EQ;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'g':
      switch (inputLookNUC (1))
	{
	case 'e':
	  cc = GE;
	  inputSkipN (2);
	  break;
	case 't':
	  cc = GT;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'h':
      switch (inputLookNUC (1))
	{
	case 'i':
	  cc = HI;
	  inputSkipN (2);
	  break;
	case 's':
	  cc = HS;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'l':
      switch (inputLookNUC (1))
	{
	case 'e':
	  cc = LE;
	  inputSkipN (2);
	  break;
	case 'o':
	  cc = LO;
	  inputSkipN (2);
	  break;
	case 's':
	  cc = LS;
	  inputSkipN (2);
	  break;
	case 't':
	  cc = LT;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'm':
      switch (inputLookNUC (1))
	{
	case 'i':
	  cc = MI;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'n':
      switch (inputLookNUC (1))
	{
	case 'e':
	  cc = NE;
	  inputSkipN (2);
	  break;
	case 'v':
	  cc = NV;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'p':
      switch (inputLookNUC (1))
	{
	case 'l':
	  cc = PL;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'v':
      switch (inputLookNUC (1))
	{
	case 'c':
	  cc = VC;
	  inputSkipN (2);
	  break;
	case 's':
	  cc = VS;
	  inputSkipN (2);
	  break;
	}
      break;
    }
  return cc;
}


static WORD
getDir (BOOL ldm)
{
  WORD dir = optionError;
  switch (inputLookUC ())
    {
    case 'd':
      switch (inputLookNUC (1))
	{
	case 'b':
	  dir = DB;
	  inputSkipN (2);
	  break;
	case 'a':
	  dir = DA;
	  inputSkipN (2);
	  break;
	}
      break;
    case 'e':
      switch (inputLookNUC (1))
	{
	case 'd':
	  dir = (ldm ? IB : DA);
	  inputSkipN (2);
	  break;
	case 'a':
	  dir = (ldm ? DB : IA);
	  inputSkipN (2);
	  break;
	}
      break;
    case 'f':
      switch (inputLookNUC (1))
	{
	case 'd':
	  dir = (ldm ? IA : DB);
	  inputSkipN (2);
	  break;
	case 'a':
	  dir = (ldm ? DA : IB);
	  inputSkipN (2);
	  break;
	}
      break;
    case 'i':
      switch (inputLookNUC (1))
	{
	case 'b':
	  dir = IB;
	  inputSkipN (2);
	  break;
	case 'a':
	  dir = IA;
	  inputSkipN (2);
	  break;
	}
      break;
    }
  return dir;
}


static WORD
getPrec (int P)
{
  switch (inputGetUC ())
    {
    case 's':
      return P ? PRECISION_MEM_SINGLE : PRECISION_SINGLE;
    case 'd':
      return P ? PRECISION_MEM_DOUBLE : PRECISION_DOUBLE;
    case 'e':
      return P ? PRECISION_MEM_EXTENDED : PRECISION_EXTENDED;
    case 'p':
      return P ? PRECISION_MEM_PACKED : optionError;
    }
  return optionError;
}


static WORD
getRound (void)
{
  switch (inputLookUC ())
    {
    case 'p':
      inputSkip ();
      return ROUND_PLUSINF;
    case 'm':
      inputSkip ();
      return ROUND_MINUSINF;
    case 'z':
      inputSkip ();
      return ROUND_ZERO;
    }
  return ROUND_NEAREST;
}


static WORD
isOK (WORD option)
{
  if (inputLook () && !isspace (inputGet ()))
    return optionError;
  else
    return option;
}


WORD
optionCond (void)
{
  return isOK (getCond ());
}


WORD
optionCondS (void)
{
  WORD option = getCond ();
  if (inputLookUC () == 's')
    {
      option |= S_FLAG;
      inputSkip ();
    }
  return isOK (option);
}


WORD
optionCondSP (void)
{
  WORD option = getCond () | S_FLAG;
  if (inputLookUC () == 's')
    {
      inputSkip ();
      if (option_pedantic)
	error (ErrorInfo, "S is implicit in test instructions");
    }
  if (inputLookUC () == 'p')
    {
      option |= P_FLAG;
      inputSkip ();
    }
  return isOK (option);
}


WORD
optionCondB (void)
{
  WORD option = getCond ();
  if (inputLookUC () == 'b')
    {
      option |= B_FLAG;
      inputSkip ();
    }
  return isOK (option);
}


WORD
optionCondBT (void)		/* also does signed byte,
				   (un)signed halfword and doubleword */
{
  WORD option = getCond ();
  if (inputLookUC () == 's')
    {
      option |= 0xD0;
      inputSkip ();
      switch (inputLookUC ())
	{
	case 'h':
	  option |= 0x20;	/* fall through to 'b' */
	case 'b':
	  inputSkip ();
	  break;
	default:
	  option = optionError;
	  break;
	}
    }
  else
    {
      switch (inputLookUC ())
	{
	case 'd':
	  option |= 0xD0 | (1 << 27);	/* Use bit 27 as a flag
					 * for doubleword access */
	  inputSkip ();
	  break;
	case 'h':
	  option |= 0xB0;
	  inputSkip ();
	  break;
	case 'b':
	  option |= B_FLAG;
	  inputSkip ();
	  break;
	}
    }
  if (inputLookUC () == 't')
    {
      option |= T_FLAG;
      inputSkip ();
    }
  return isOK (option);
}


WORD
optionCondDirLdm (void)
{
  WORD option = getCond ();
  if (optionError == (option |= getDir (TRUE)))
    return optionError;
  return isOK (option);
}


WORD
optionCondDirStm (void)
{
  WORD option = getCond ();
  if (optionError == (option |= getDir (FALSE)))
    return optionError;
  return isOK (option);
}


WORD
optionCondLfmSfm (void)
{
  return getCond ();
}


WORD
optionCondPrecRound (void)
{
  WORD option = getCond ();
  if (optionError == (option |= getPrec (FALSE)))
    return optionError;
  return isOK (option | getRound ());
}


WORD
optionCondOptPrecRound (void)
{
  WORD optionCC = getCond ();
  WORD optionPrec = getPrec (FALSE);
  if (optionError == optionPrec)
    optionPrec = PRECISION_EXTENDED;
  return isOK (optionCC | optionPrec | getRound ());
}


WORD
optionCondPrec_P (void)
{
  WORD option = getCond ();
  if (optionError == (option |= getPrec (TRUE)))
    return optionError;
  return isOK (option);
}


WORD
optionCondL (void)
{
  WORD option = getCond ();
  if (inputLookUC () == 'l')
    {
      option |= L_FLAG;
      inputSkip ();
    }
  return isOK (option);
}


/****** Trouble mnemonics **********/



WORD
optionCondOptRound (void)
{
  WORD optionCC = getCond ();
  return isOK (optionCC | PRECISION_SINGLE | getRound ());
}


WORD
optionLinkCond (void)		/* 'b' is matched before call */
{
  if (inputLookUC () != 'l')
    {				/* Only b.CC possible  */
      return isOK (getCond ());
    }
  else
    {
      inputSkip ();		/* bl.CC or b.l?  */
      switch (inputLookUC ())
	{
	case 'e':		/* b.le or bl.eq */
	  inputSkip ();
	  switch (inputLookUC ())
	    {
	    case 'q':		/* Only bl.eq */
	      inputSkip ();
	      return isOK (EQ | LINK_BIT);
	    default:		/* Only b.le */
	      return isOK (LE);
	    }
	case 'o':		/* Only b.lo possible */
	  inputSkip ();
	  return isOK (LO);
	case 's':		/* Only b.ls possible */
	  inputSkip ();
	  return isOK (LS);
	case 't':		/* Only b.lt possible */
	  inputSkip ();
	  return isOK (LT);
	default:		/* Only bl.CC possible */
	  return isOK (getCond () | LINK_BIT);
	}
    }
  return optionError;
}


WORD
optionExceptionCond (void)
{
  if (inputLookUC () != 'e')
    {				/* Only cmf.CC possible  */
      return isOK (getCond ());
    }
  else
    {				/* cmf.eq or cmfe.CC */
      inputSkip ();
      if (inputLookUC () == 'q')
	{			/* Only cmf.eq */
	  inputSkip ();
	  return isOK (EQ);
	}
      else
	{			/* Only cmfe.CC */
	  return isOK (getCond () | EXEPTION_BIT);
	}
    }
  return optionError;
}


WORD
optionAdrL (void)
{
  WORD option = getCond ();
  if (inputLookUC () == 'l')
    {
      option |= 1;
      inputSkip ();
    }
  return isOK (option);
}
