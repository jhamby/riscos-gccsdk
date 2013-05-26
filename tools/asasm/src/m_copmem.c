/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "error.h"
#include "get.h"
#include "help_cop.h"
#include "input.h"
#include "it.h"
#include "main.h"
#include "m_copmem.h"
#include "option.h"
#include "state.h"
#include "targetcpu.h"


static bool
dstmem (const char *descr, bool doLowerCase, bool two,
        ARMWord ir, bool literal)
{
  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (ir, instrState != eInstrType_ARM); 

  /* For Thumb2, we need at least ARMv6T2.
     CDP2, MCR2, MRC2 for ARM need at least ARMv5T.  */
  if (instrState != eInstrType_ARM)
    Target_CheckCPUFeature (kCPUExt_v6T2, true);
  else if (two)
    Target_CheckCPUFeature (kCPUExt_v5T, true);

  int cop = CP_NUMBER (Get_CopNum ());

  /* FPA uses coprocessor 1 and 2 (the latter only for LFM/SFM).  */
  if (cop == CP_NUMBER (1) || cop == CP_NUMBER (2))
    {
      if (option_pedantic)
	Error (ErrorInfo, "Coprocessor 1 and 2 are FPA floating point unit. Use FPA mnemonics if possible");
    }
  ir |= cop;

  Input_SkipWS ();
  if (!Input_Match (',', true))
    Error (ErrorError, "%scoprocessor number", InsertCommaAfter);
  ir |= CPDST_OP (Get_CopReg ());

  if (instrState != eInstrType_ARM && instrWidth == eInstrWidth_Enforce16bit)
    Error (ErrorError, "Instruction %s is not supported for Thumb", descr);

  /* Instructions bits for ARM and Thumb2 are the same.  Some cc fiddling
     is still needed.  */
  if (two)
    ir |= 0xf0000000;
  else if (instrState != eInstrType_ARM)
    ir = (ir & 0x0fffffff) | 0xe0000000;
  HelpCop_Addr (ir, literal, false);
  return false;
}

/**
 * Implements LDC and LDC2.
 * LDC{<cond>}{L} <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 * LDC{L}{<cond>} <coproc>, <CRd>, <addressing mode 5> (UAL)
 * LDC2{L} <coproc>, <CRd>, <addressing mode 5>
 */
bool
m_ldc (bool doLowerCase)
{
  if (Input_Match ('2', false))
    return dstmem ("LDC2", doLowerCase, true,
                   0xe0000000 | (Input_Match (doLowerCase ? 'l' : 'L', false) ? N_FLAG : 0) | 0x0c100000, true);

  ARMWord cc = Option_CondL (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstmem ("LDC", doLowerCase, false, cc | 0x0c100000, true);
}

/**
 * Implements STC.
 * STC{<cond>}{L} <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 * STC{L}{<cond>} <coproc>, <CRd>, <addressing mode 5> (pre-UAL)
 * STC2{L} <coproc>, <CRd>, <addressing mode 5>
 */
bool
m_stc (bool doLowerCase)
{
  if (Input_Match ('2', false))
    return dstmem ("STC2", doLowerCase, true,
                   0xe0000000 | (Input_Match (doLowerCase ? 'l' : 'L', false) ? N_FLAG : 0) | 0x0c000000, false);

  ARMWord cc = Option_CondL (doLowerCase);
  if (cc == kOption_NotRecognized)
    return true;
  return dstmem ("STC", doLowerCase, false, cc | 0x0c000000, false);
}
