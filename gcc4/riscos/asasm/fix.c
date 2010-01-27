/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2010 GCCSDK Developers
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
 * fix.c
 */

#include <stdlib.h>

#include "error.h"
#include "fix.h"
#include "global.h"
#include "help_cpu.h"
#include "main.h"
#include "m_cpu.h"
#include "m_fpu.h"
#include "option.h"

WORD
fixShiftImm (int lineno, WORD shiftop, int shift)
{
  switch (shiftop)
    {
    case LSL:
      if (shift < 0 || shift > 31)
	{
	  errorLine (lineno, NULL, ErrorError, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    case LSR:
      if (shift == 0)
	shiftop = LSL;
      else if (shift < 1 || shift > 32)
	{
	  errorLine (lineno, NULL, ErrorError, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    case ASR:
      if (shift == 0)
	shiftop = LSL;
      else if (shift < 1 || shift > 32)
	{
	  errorLine (lineno, NULL, ErrorError, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    case ROR:
      if (shift == 0)
	shiftop = LSL;
      else if (shift < 1 || shift > 31)
	{
	  errorLine (lineno, NULL, ErrorError, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    default:
      errorAbortLine (lineno, NULL, "Internal fixShiftImm: unknown shift type");
      break;
    }
  return SHIFT_IMM (shift) | SHIFT_OP (shiftop);
}

WORD
fixImm8s4 (int lineno, WORD ir, int im)
{
  static const char op3[] = "Changing \"%s R_, R_, #%d\" to \"%s R_, R_, #%d\"";
  static const char op2[] = "Changing \"%s R_, #%d\" to \"%s R_, #%d\"";
  const char *m1, *m2, *optype;
  int i8s4;
  WORD mnemonic;
  int im2;

  i8s4 = help_cpuImm8s4 (im);
  if (i8s4 != -1)
    return ir | i8s4;

  mnemonic = ir & M_MNEM;

  /* Immediate constant was illegal.  Try the inverse or
     two complement (depending on opcode).  */
  switch (mnemonic)
    {
    case M_ADD:
    case M_SUB:
    case M_ADC:
    case M_SBC:
    case M_CMP:
    case M_CMN:
      im2 = -im;
      break;
    case M_MOV:
    case M_MVN:
    case M_AND:
    case M_BIC:
      im2 = ~im;
      break;
    default:
      im2 = im;
      break;
    }
  i8s4 = help_cpuImm8s4 (im2);
  if (i8s4 == -1)
    {
      errorLine (lineno, NULL, ErrorError,
		 "Illegal immediate constant %d (0x%08x)", im, im);
      return ir;
    }

  ir &= ~M_MNEM;
  switch (mnemonic)
    {			/* try changing opcode */
    case M_ADD:
      ir |= M_SUB;
      optype = op3; m1 = "ADD"; m2 = "SUB";
      break;
    case M_SUB:
      ir |= M_ADD;
      optype = op3; m1 = "SUB"; m2 = "ADD";
      break;
    case M_ADC:
      ir |= M_SBC;
      optype = op3; m1 = "ADC"; m2 = "SBC";
      break;
    case M_SBC:
      ir |= M_ADC;
      optype = op3; m1 = "SBC"; m2 = "ADC";
      break;
    case M_CMP:
      ir |= M_CMN;
      optype = op2; m1 = "CMP"; m2 = "CMN";
      break;
    case M_CMN:
      ir |= M_CMP;
      optype = op2; m1 = "CMN"; m2 = "CMP";
      break;
    case M_MOV:
      ir |= M_MVN;
      optype = op2; m1 = "MOV"; m2 = "MVN";
      break;
    case M_MVN:
      ir |= M_MOV;
      optype = op2; m1 = "MVN"; m2 = "MOV";
      break;
    case M_AND:
      ir |= M_BIC;
      optype = op3; m1 = "AND"; m2 = "BIC";
      break;
    case M_BIC:
      ir |= M_AND;
      optype = op3; m1 = "BIC"; m2 = "AND";
      break;
    default:
      errorAbortLine (lineno, NULL, "Internal fixImm8s4: unknown mnemonic");
      return ir;
   }

  if (option_fussy > 1)
    errorLine (lineno, NULL, ErrorInfo, optype, m1, im, m2, im2);

  return ir | i8s4;
}

WORD
fixImmFloat (int lineno, WORD ir, FLOAT im)
{
  static const char op3[] = "Changing \"%s F_, F_, #%.1f\" to \"%s F_, F_, #%.1f\"";
  static const char op2[] = "Changing \"%s F_, #%.1f\" to \"%s F_, #%.1f\"";
  const char *m1, *m2, *optype;
  int f;
  WORD mnemonic;

  f = fpuImm (im);
  if (f != -1)
    return ir | f;

  /* Immediate float constant was illegal.  */
  f = fpuImm (-im);
  if (f == -1)
    {
      /* Even the inverse cannot be represented.  */
      errorLine (lineno, NULL, ErrorError,
		 "Illegal immediate constant %g", im);
      return ir;
    }

  /* Inverse immediate constant can be represented, so try to invert
     the mnemonic.  */
  mnemonic = ir & M_FMNEM;
  ir &= ~M_FMNEM;
  switch (mnemonic)
    {
    case M_ADF:
      ir |= M_SUF;
      optype = op3; m1 = "ADF"; m2 = "SUF";
      break;
    case M_SUF:
      ir |= M_ADF;
      optype = op3; m1 = "SUF"; m2 = "ADF";
      break;
    case M_MVF:
      ir |= M_MNF;
      optype = op2; m1 = "MVF"; m2 = "MNF";
      break;
    case M_MNF:
      ir |= M_MVF;
      optype = op2; m1 = "MNF"; m2 = "MVF";
      break;
    case M_CMF & M_FMNEM:
      ir |= M_CNF;
      optype = op2; m1 = "CMF"; m2 = "CNF";
      break;
    case M_CNF & M_FMNEM:
      ir |= M_CMF;
      optype = op2; m1 = "CNF"; m2 = "CMF";
      break;
    case (M_CMF | EXEPTION_BIT) & M_FMNEM:
      ir |= M_CNF | EXEPTION_BIT;
      optype = op2; m1 = "CMFE"; m2 = "CNFE";
      break;
    case (M_CNF | EXEPTION_BIT) & M_FMNEM:
      ir |= M_CMF | EXEPTION_BIT;
      optype = op2; m1 = "CNFE"; m2 = "CMFE";
      break;
    default:
      errorAbortLine (lineno, NULL, "Internal fixImmFloat: unknown mnemonic");
      return ir;
      break;
    }

  if (option_fussy > 1)
    errorLine (lineno, NULL, ErrorInfo, optype, m1, im, m2, -im);

  return ir | f;
}

WORD
fixAdr (int lineno, WORD ir, int im)	/* !!! mov and mvn should be possible */
{
  int i8s4;
  if (im < 0)
    {
      ir |= 0x00400000;		/* sub */
      im = -im;
    }
  else
    ir |= 0x00800000;		/* add */
  i8s4 = help_cpuImm8s4 (im);
  if (i8s4 == -1)
    errorLine (lineno, NULL, ErrorError, "Offset %d (0x%08x) is illegal for adr", im, im);
  else
    ir |= i8s4;
  return ir;
}

void
fixAdrl (int lineno, WORD * ir, WORD * ir2, int im, int warn)
{
  int i8s4;
  if (im < 0)
    {
      *ir |= 0x00400000;	/* sub */
      im = -im;
    }
  else
    *ir |= 0x00800000;		/* add */
  *ir2 = (*ir & 0xFFF0F000) | ((*ir & 0x0000F000) << 4);
  i8s4 = help_cpuImm8s4 (im);
  if (i8s4 != -1)
    {
      if (warn && option_fussy && (im == 0 || (*ir & 0x00400000) || help_cpuImm8s4 (im - 4) != -1))
	errorLine (lineno, NULL, ErrorInfo, "ADRL not required for offset %d (0x%08x)", im, im);
      *ir |= i8s4;
    }
  else
    {
      int shift = 0;
      while ((im & 3 << shift) == 0)
	shift += 2;		/* shift Doesn't Exceed 24 */
      *ir |= (32 - (shift & 30)) << 7 | (im >> (shift & 0xFF));
      i8s4 = help_cpuImm8s4 (im & ~(0xFF << shift));
      if (i8s4 == -1)
	errorLine (lineno, NULL, ErrorError, "Offset %d (0x%08x) is illegal for adrl", im, im);
      else
	*ir2 |= i8s4;
    }
}

WORD
fixSwi (int lineno, int im)
{
  if ((im & 0xffffff) != im)
    errorLine (lineno, NULL, ErrorError, "Illegal swi number %d(0x%08x)", im, im);
  return im & 0xffffff;
}

WORD
fixBranch (int lineno, int im)
{
  if (im & 3)
    errorLine (lineno, NULL, ErrorError, "Branch value is not a multiple of four");
  return (im >> 2) & 0xffffff;
}

WORD
fixBranchT (int lineno, int im)
{
  if (im & 1)
    errorLine (lineno, NULL, ErrorError, "Branch value is not a multiple of two");

  return ((im >> 2) & 0xffffff) | ((im & 2) << 23);
}

WORD
fixCopOffset (int lineno, WORD ir, int offset)
{
  BOOL up;
  if (offset < 0)
    {
      offset = -offset;
      up = FALSE;
    }
  else
    up = TRUE;
  if (offset & 3)
    errorLine (lineno, NULL, ErrorError, "Offset %d is not a word offset", offset);
  if (offset > 1020)
    errorLine (lineno, NULL, ErrorError, "Offset %d is too large", offset);
  ir |= (offset >> 2) & 0xff;
  if (up)
    ir |= UP_FLAG;
  return ir;
}

WORD
fixCpuOffset (int lineno, WORD ir, int offset)
{
  BOOL up;
  if (offset < 0)
    {
      offset = -offset;
      up = FALSE;
    }
  else
    up = TRUE;
  if ((ir & 0x90) == 0x90)
    {
      if (offset > 255)
	errorLine (lineno, NULL, ErrorError, "Offset %d is too large", offset);
      ir |= (offset & 0xF) | (offset & 0xF0) << 4;
    }
  else
    {
      if (offset > 4095)
	errorLine (lineno, NULL, ErrorError, "Offset %d is too large", offset);
      ir |= offset & 0xfff;
    }
  if (up)
    ir |= UP_FLAG;
  return ir;
}

WORD
fixMask (int lineno, int mask)
{
  if (mask < 0 || mask > 0xffff)
    errorLine (lineno, NULL, ErrorError, "Illegal value for register mask 0x%x", mask);
  return mask & 0xffff;
}

/**
 * Check if given value fits in word of given size.
 * \entry lineno Source linenumber where the value is coming from.
 * \entry size Size in bytes of word value, should be 1, 2 or 4.
 * \entry value Value which should be checked for.
 * When the check fails, an error is given and the value is truncated so it
 * fits in given word size.
 */
WORD
fixInt (int lineno, int size, int value)
{
  switch (size)
    {
      case 1:
	if (value < -128 || value >= 256)
	  {
	    errorLine (lineno, NULL, ErrorError, "Expression %d too big for %i bits", value, 8);
	    value &= 0xff;
	  }
	break;
      case 2:
	if (value < -32768 || value >= 65536)
	  {
	    errorLine (lineno, NULL, ErrorError, "Expression %d too big for %i bits", value, 16);
	    value &= 0xffff;
	  }
	break;
      case 4:
	break;
      default:
	errorAbortLine (lineno, NULL, "Internal fixInt: size %d is not legal", size);
	break;
    }
  return value;
}
