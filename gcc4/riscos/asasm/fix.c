/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2012 GCCSDK Developers
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

#include <assert.h>
#include <stdlib.h>

#include "error.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "help_cpu.h"
#include "main.h"
#include "m_cpu.h"
#include "option.h"

ARMWord
Fix_ShiftImm (const char *fileName, unsigned lineNum, ARMWord shiftop, int shift)
{
  if (shift == 0)
    shiftop = LSL;
  switch (shiftop)
    {
      case LSL:
        if (shift < 0 || shift > 31)
	  {
	    errorLine (fileName, lineNum, ErrorError, "Illegal immediate shift %d", shift);
	    shift = 0;
	  }
        break;
      case LSR:
        if (shift < 1 || shift > 32)
	  {
	    errorLine (fileName, lineNum, ErrorError, "Illegal immediate shift %d", shift);
	    shift = 1;
	  }
        break;
      case ASR:
        if (shift < 1 || shift > 32)
	  {
	    errorLine (fileName, lineNum, ErrorError, "Illegal immediate shift %d", shift);
	    shift = 1;
	  }
        break;
      case ROR:
        if (shift < 1 || shift > 31)
	  {
	    errorLine (fileName, lineNum, ErrorError, "Illegal immediate shift %d", shift);
	    shift = 1;
	  }
        break;
      default:
        errorAbortLine (fileName, lineNum, "Internal fixShiftImm: unknown shift type");
        break;
    }
  return SHIFT_IMM (shift) | SHIFT_OP (shiftop);
}

ARMWord
fixImm8s4 (unsigned lineNum, ARMWord ir, int im)
{
  static const char op3[] = "Changing \"%s Rx, Ry, #%d\" to \"%s Rx, Ry, #%d\"";
  static const char op2[] = "Changing \"%s Rx, #%d\" to \"%s Rx, #%d\"";

  int i8s4 = help_cpuImm8s4 (im);
  if (i8s4 != -1)
    return ir | i8s4;

  ARMWord mnemonic = ir & M_MNEM;

  /* Immediate constant was illegal.  Try the inverse or
     two complement (depending on opcode).  */
  int im2;
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
      errorLine (NULL, lineNum, ErrorError,
		 "Illegal immediate constant %d (0x%08x)", im, im);
      return ir;
    }

  ir &= ~M_MNEM;
  const char *m1, *m2, *optype;
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
	errorAbortLine (NULL, lineNum, "Internal fixImm8s4: unknown mnemonic");
	return ir;
    }
  
  if (option_fussy)
    errorLine (NULL, lineNum, ErrorInfo, optype, m1, im, m2, im2);

  return ir | i8s4;
}

ARMWord
Fix_MOV (const char *fileName, unsigned lineNum, ARMWord ir, int im)
{
  if (im < 0)
    {
      ir |= M_MVN;
      im = ~im;
    }
  else
    ir |= M_MOV;
  int i8s4 = help_cpuImm8s4 (im);
  if (i8s4 == -1)
    errorLine (fileName, lineNum, ErrorError, "Offset %d (0x%08x) is illegal for ADR", im, im);
  else
    ir |= i8s4;
  return ir;
}

ARMWord
fixSwi (unsigned lineNum, int im)
{
  if ((im & 0xffffff) != im)
    errorLine (NULL, lineNum, ErrorError, "Illegal swi number %d(0x%08x)", im, im);
  return im & 0xffffff;
}

ARMWord
Fix_CopOffset (const char *fileName, unsigned lineNum, ARMWord ir, int offset)
{
  bool up;
  if (offset < 0)
    {
      offset = -offset;
      up = false;
    }
  else
    up = true;
  if (offset & 3)
    errorLine (fileName, lineNum, ErrorError, "Offset %d is not a word offset", offset);
  if (offset > 1020)
    errorLine (fileName, lineNum, ErrorError, "Offset %d is too large", offset);
  ir |= (offset >> 2) & 0xff;
  if (up)
    ir |= U_FLAG;
  return ir;
}

ARMWord
Fix_CPUOffset (const char *fileName, unsigned lineNum, ARMWord ir, int offset)
{
  bool up;
  if (offset < 0)
    {
      offset = -offset;
      up = false;
    }
  else
    up = true;
  bool isAddrMode3 = (ir & 0x04000090) == 0x90;
  if (isAddrMode3)
    {
      if (offset > 255)
	errorLine (fileName, lineNum, ErrorError, "Offset %d is too large", offset);
      ir |= (offset & 0xF) | (offset & 0xF0) << 4;
    }
  else
    {
      if (offset > 4095)
	errorLine (fileName, lineNum, ErrorError, "Offset %d is too large", offset);
      ir |= offset & 0xfff;
    }
  if (up)
    ir |= U_FLAG;
  return ir;
}

ARMWord
fixMask (unsigned lineNum, int mask)
{
  if (mask < 0 || mask > 0xffff)
    errorLine (NULL, lineNum, ErrorError, "Illegal value for register mask 0x%x", mask);
  return mask & 0xffff;
}

/**
 * Check if given value fits in word of given size.
 * \entry lineNum Source linenumber where the value is coming from.
 * \entry size Size in bytes of word value, should be 1, 2 or 4.
 * \entry value Value which should be checked for.
 * When the check fails, an error is given and the value is truncated so it
 * fits in given word size.
 */
ARMWord
Fix_Int (const char *fileName, unsigned lineNum, int size, int value)
{
  switch (size)
    {
      case 1:
	if (value < -128 || value >= 256)
	  {
	    errorLine (fileName, lineNum, ErrorError, "Expression %d too big for %i bits", value, 8);
	    value &= 0xff;
	  }
	break;

      case 2:
	if (value < -32768 || value >= 65536)
	  {
	    errorLine (fileName, lineNum, ErrorError, "Expression %d too big for %i bits", value, 16);
	    value &= 0xffff;
	  }
	break;

      case 4:
	break;

      default:
	assert (0);
	break;
    }
  return value;
}
