/*
 * fix.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "error.h"
#include "fix.h"
#include "global.h"
#include "help_cpu.h"
#include "main.h"
#include "m_cpu.h"
#include "m_fpu.h"
#include "option.h"

WORD
fixShiftImm (long int lineno, WORD shiftop, int shift)
{
  switch (shiftop)
    {
    case LSL:
      if (shift < 0 || shift > 31)
	{
	  errorLine (lineno, NULL, ErrorError, TRUE, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    case LSR:
      if (shift == 0)
	shiftop = LSL;
      else if (shift < 1 || shift > 32)
	{
	  errorLine (lineno, NULL, ErrorError, TRUE, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    case ASR:
      if (shift == 0)
	shiftop = LSL;
      else if (shift < 1 || shift > 32)
	{
	  errorLine (lineno, NULL, ErrorError, TRUE, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    case ROR:
      if (shift == 0)
	shiftop = LSL;
      else if (shift < 1 || shift > 31)
	{
	  errorLine (lineno, NULL, ErrorError, TRUE, "Illegal immediate shift %d", shift);
	  shift = 1;
	}
      break;
    default:
      errorLine (lineno, NULL, ErrorSerious, FALSE, "Internal fixShiftImm: unknown shift type");
      break;
    }
  return SHIFT_IMM (shift) | SHIFT_OP (shiftop);
}

WORD
fixImm8s4 (long int lineno, WORD ir, int im)
{
  int i8s4 = help_cpuImm8s4 (im);
  if (i8s4 == -1)
    {				/* Not a legal immediate */
      switch (ir & M_MNEM)
	{			/* try changing opcode */
	case M_ADD:
	  if ((i8s4 = help_cpuImm8s4 (-im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_SUB;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,r_,#%d to %s r_,r_,#%d", "add", im, "sub", -im);
	    }
	  break;
	case M_SUB:
	  if ((i8s4 = help_cpuImm8s4 (-im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_ADD;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,r_,#%d to %s r_,r_,#%d", "sub", im, "add", -im);
	    }
	  break;
	case M_ADC:
	  if ((i8s4 = help_cpuImm8s4 (-im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_SBC;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,r_,#%d to %s r_,r_,#%d", "adc", im, "sbc", -im);
	    }
	  break;
	case M_SBC:
	  if ((i8s4 = help_cpuImm8s4 (-im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_ADC;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,r_,#%d to %s r_,r_,#%d", "sbc", im, "adc", -im);
	    }
	  break;
	case M_CMP:
	  if ((i8s4 = help_cpuImm8s4 (-im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_CMN;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,#%d to %s r_,#%d", "cmp", im, "cmn", -im);
	    }
	  break;
	case M_CMN:
	  if ((i8s4 = help_cpuImm8s4 (-im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_CMP;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,#%d to %s r_,#%d", "cmn", im, "cmp", -im);
	    }
	  break;
	case M_MOV:
	  if ((i8s4 = help_cpuImm8s4 (~im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_MVN;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,#%d to %s r_,#%d", "mov", im, "mvn", ~im);
	    }
	  break;
	case M_MVN:
	  if ((i8s4 = help_cpuImm8s4 (~im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_MOV;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,#%d to %s r_,#%d", "mvn", im, "mov", ~im);
	    }
	  break;
	case M_AND:
	  if ((i8s4 = help_cpuImm8s4 (~im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_BIC;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,r_,#0x%x to %s r_,r_,#0x%x", "and", im, "bic", ~im);
	    }
	  break;
	case M_BIC:
	  if ((i8s4 = help_cpuImm8s4 (~im)) != -1)
	    {
	      ir = (ir & ~M_MNEM) | M_AND;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s r_,r_,#0x%x to %s r_,r_,#0x%x", "bic", im, "and", ~im);
	    }
	  break;
	}
    }
  if (i8s4 == -1)
    {
      errorLine (lineno, NULL, ErrorError, TRUE, "Illegal immediate constant %d (0x%08x)", im, im);
      i8s4 = 0;
    }
  return ir | i8s4;
}

WORD
fixImmFloat (long int lineno, WORD ir, FLOAT im)
{
  int f = fpuImm (im);
  if (f == -1)
    {				/* Not a legal float immediate */
      switch (ir & M_FMNEM)
	{			/* try changing opcode */
	case M_ADF:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_SUF;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,f_,#%.1f to %s f_,f_,#%.1f", "adf", im, "suf", -im);
	    }
	  break;
	case M_SUF:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_ADF;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,f_,#%.1f to %s f_,f_,#%.1f", "suf", im, "adf", -im);
	    }
	  break;
	case M_MVF:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_MNF;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,#%.1f to %s f_,#%.1f", "mvf", im, "mnf", -im);
	    }
	  break;
	case M_MNF:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_MVF;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,#%.1f to %s f_,#%.1f", "mnf", im, "mvf", -im);
	    }
	  break;
	case M_CMF & M_FMNEM:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_CNF;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,#%.1f to %s f_,#%.1f", "cmf", im, "cnf", -im);
	    }
	  break;
	case M_CNF & M_FMNEM:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_CMF;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,#%.1f to %s f_,#%.1f", "cnf", im, "cmf", -im);
	    }
	  break;
	case (M_CMF | EXEPTION_BIT) & M_FMNEM:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_CNF | EXEPTION_BIT;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,#%.1f to %s f_,#%.1f", "cmfe", im, "cnfe", -im);
	    }
	  break;
	case (M_CNF | EXEPTION_BIT) & M_FMNEM:
	  if ((f = fpuImm (-im)) != -1)
	    {
	      ir = (ir & ~M_FMNEM) | M_CMF | EXEPTION_BIT;
	      if (fussy > 1)
		errorLine (lineno, NULL, ErrorInfo, TRUE, "Changing %s f_,#%.1f to %s f_,#%.1f", "cnfe", im, "cmfe", -im);
	    }
	  break;
	}
    }
  if (f == -1)
    {
      errorLine (lineno, NULL, ErrorError, TRUE, "Illegal immediate constant %g", im);
      f = 0;
    }
  return ir | f;
}

WORD
fixAdr (long int lineno, WORD ir, int im)	/* !!! mov and mvn should be possible */
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
    errorLine (lineno, NULL, ErrorError, TRUE, "Offset %d (0x%08x) is illegal for adr", im, im);
  else
    ir |= i8s4;
  return ir;
}

void
fixAdrl (long int lineno, WORD * ir, WORD * ir2, int im, int warn)
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
      if (warn && fussy && (im == 0 || (*ir & 0x00400000) || help_cpuImm8s4 (im - 4) != -1))
	errorLine (lineno, NULL, ErrorInfo, TRUE, "ADRL not required for offset %d (0x%08x)", im, im);
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
	errorLine (lineno, NULL, ErrorError, TRUE, "Offset %d (0x%08x) is illegal for adrl", im, im);
      else
	*ir2 |= i8s4;
    }
}

WORD
fixSwi (long int lineno, int im)
{
  if ((im & 0xffffff) != im)
    errorLine (lineno, NULL, ErrorError, TRUE, "Illegal swi number %d(0x%08x)", im, im);
  return im & 0xffffff;
}

WORD
fixBranch (long int lineno, int im)
{
  if (im & 3)
    errorLine (lineno, NULL, ErrorError, TRUE, "Branch value is not a multiple of four");
  return (im >> 2) & 0xffffff;
}

WORD
fixCopOffset (long int lineno, WORD ir, int offset)
{
  BOOL up = TRUE;
  if (offset < 0)
    {
      offset = -offset;
      up = FALSE;
    }
  if (offset & 3)
    errorLine (lineno, NULL, ErrorError, TRUE, "Offset %d is not a word offset", offset);
  if (offset > 1020)
    errorLine (lineno, NULL, ErrorError, TRUE, "Offset %d is too large", offset);
  ir |= (offset >> 2) & 0xff;
  if (up)
    ir |= UP_FLAG;
  return ir;
}

WORD
fixCpuOffset (long int lineno, WORD ir, int offset)
{
  BOOL up = TRUE;
  if (offset < 0)
    {
      offset = -offset;
      up = FALSE;
    }
  if ((ir & 0x90) == 0x90)
    {
      if (offset > 255)
	errorLine (lineno, NULL, ErrorError, TRUE, "Offset %d is too large", offset);
      ir |= (offset & 0xF) | (offset & 0xF0) << 4;
    }
  else
    {
      if (offset > 4095)
	errorLine (lineno, NULL, ErrorError, TRUE, "Offset %d is too large", offset);
      ir |= offset & 0xfff;
    }
  if (up)
    ir |= UP_FLAG;
  return ir;
}

WORD
fixMask (long int lineno, int mask)
{
  if (mask < 0 || mask > 0xffff)
    errorLine (lineno, NULL, ErrorError, TRUE, "Illegal value for register mask 0x%x", mask);
  return mask & 0xffff;
}

WORD
fixInt (long int lineno, int size, int value)
{
  switch (size)
    {
    case 1:
      if (value < -128 || value > 256)
	{
	  errorLine (lineno, NULL, ErrorError, TRUE, "Expression %d too big for %i bits", value, 8);
	  value &= 0xff;
	}
      break;
    case 2:
      if (value < -32768 || value > 65536)
	{
	  errorLine (lineno, NULL, ErrorError, TRUE, "Expression %d too big for %i bits", value, 16);
	  value &= 0xffff;
	}
      break;
    case 4:
      break;
    default:
      errorLine (lineno, NULL, ErrorSerious, TRUE, "Internal fixInt: size %d is not legal", size);
      break;
    }
  return value;
}
