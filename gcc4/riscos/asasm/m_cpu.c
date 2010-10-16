/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
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
 * m_cpu.c
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "error.h"
#include "expr.h"
#include "get.h"
#include "global.h"
#include "input.h"
#include "main.h"
#include "mnemonics.h"
#include "m_cpu.h"
#include "option.h"
#include "put.h"
#include "targetcpu.h"

/** DATA none (or optional register) **/

void
m_nop (void)
{
  if (!inputComment ())
    {
      ARMWord op = getCpuReg ();
      if (op == 15)
	error (ErrorError, "Cannot use R15 in NOP");
      putIns (0xE1A00000 | DST_OP (op) | RHS_OP (op));
    }
  else
    putIns (0xE1A00000);
}

/** DATA dst=lhs<op>rhs **/

static void
dstlhsrhs (ARMWord ir)
{
  ARMWord op;
  op = getCpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  op = getCpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  ir = getRhs (false, true, ir);
  putIns (ir);
}

static void
dstrhs (ARMWord ir)
{
  ARMWord op;
  op = getCpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir = getRhs (false, true, ir);
  putIns (ir);
}

void
m_adc (ARMWord cc)
{
  dstlhsrhs (cc | M_ADC);
}

void
m_add (ARMWord cc)
{
  dstlhsrhs (cc | M_ADD);
}

void
m_and (ARMWord cc)
{
  dstlhsrhs (cc | M_AND);
}

void
m_bic (ARMWord cc)
{
  dstlhsrhs (cc | M_BIC);
}

void
m_eor (ARMWord cc)
{
  dstlhsrhs (cc | M_EOR);
}

void
m_mov (ARMWord cc)
{
  dstrhs (cc | M_MOV);
}

void
m_mvn (ARMWord cc)
{
  dstrhs (cc | M_MVN);
}

void
m_orr (ARMWord cc)
{
  dstlhsrhs (cc | M_ORR);
}

void
m_rsb (ARMWord cc)
{
  dstlhsrhs (cc | M_RSB);
}

void
m_rsc (ARMWord cc)
{
  dstlhsrhs (cc | M_RSC);
}

void
m_sbc (ARMWord cc)
{
  dstlhsrhs (cc | M_SBC);
}

void
m_sub (ARMWord cc)
{
  dstlhsrhs (cc | M_SUB);
}

/** DATA test **/

static void
lhsrhs (ARMWord ir)
{
  ARMWord op;
  op = getCpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  if ((ir & P_FLAG) && option_apcs_32bit)
    error (ErrorWarning, "TSTP/TEQP/CMNP/CMPP inadvisable in 32-bit PC configurations");
  ir = getRhs (false, true, ir);
  putIns (ir);
}

void
m_cmn (ARMWord cc)
{
  lhsrhs (cc | M_CMN);
}

void
m_cmp (ARMWord cc)
{
  lhsrhs (cc | M_CMP);
}

void
m_teq (ARMWord cc)
{
  lhsrhs (cc | M_TEQ);
}

void
m_tst (ARMWord cc)
{
  lhsrhs (cc | M_TST);
}

/** DATA 1a **/

static void
onlyregs (bool acc, ARMWord ir)
{
  ARMWord dst, rhs, lhs;
  dst = getCpuReg ();
  ir |= DST_MUL (dst);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  lhs = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  rhs = getCpuReg ();
  if (dst == lhs)
    {
      if (dst == rhs)
	error (ErrorError, "Destination and left operand are the same register %d", dst);
      else
	{
	  if (option_fussy)
	    error (ErrorInfo, "Changing order of operands in %s", acc ? "MLA" : "MUL");
	  int t = lhs;
	  lhs = rhs;
	  rhs = t;
	}
    }
  ir |= LHS_MUL (lhs);
  ir |= RHS_MUL (rhs);
  skipblanks ();
  if (acc)
    {
      if (inputLook () == ',')
	{
	  inputSkip ();
	  skipblanks ();
	}
      else
	error (ErrorError, "%srhs", InsertCommaAfter);
      ir |= ACC_MUL (getCpuReg ());
      skipblanks ();
    }
  putIns (ir);
}

void
m_mla (ARMWord cc)
{
  onlyregs (true, cc | M_MLA);
}

void
m_mul (ARMWord cc)
{
  onlyregs (false, cc | M_MUL);
}

static void
l_onlyregs (ARMWord ir, const char *op)
{
  ARMWord dstl, dsth, lhs, rhs;
  /* This bit only set for smulxx */
  int issmull = !(ir & 0x01000000);
  int issmlaxy = ((ir & 0x00600000) == 0);
  int issmlalxy = ((ir & 0x00600000) == 0x00400000);
  int issmlawy = ((ir & 0x00600020) == 0x00200000);

  if (issmull)
    {
      cpuWarn (ARM7M);
      skipblanks ();
      dstl = getCpuReg ();
      if (inputLook () == ',')
        {
          inputSkip ();
          skipblanks ();
        }
      else
        error (ErrorError, "%sdst_h", InsertCommaAfter);
    }
  else
    {
      cpuWarn (XSCALE);
      skipblanks ();
      if (issmlalxy)
        {
          dstl = getCpuReg ();
          if (inputLook () == ',')
            {
              inputSkip ();
              skipblanks ();
            }
          else
            error (ErrorError, "%sdst_l", InsertCommaAfter);
        }
      else
        dstl = 0;
    }

  dsth = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst_l", InsertCommaAfter);
  lhs = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  rhs = getCpuReg ();
  if (issmull)
    {
      if (dstl == dsth)
        error (ErrorError, "Destination high and low are the same register %d", dstl);
      if (dstl == lhs || dsth == lhs)
        {
          if (dstl == rhs || dsth == rhs)
            error (ErrorError, "Left operand register %d also occurs in destination", lhs);
          else
	    {
	      if (option_fussy)
	        error (ErrorInfo, "Changing order of operands in %s", op);
	      int t = lhs;
	      lhs = rhs;
	      rhs = t;
	    }
        }
     }
  else
     {
       if (dsth == 15 || lhs == 15 || rhs == 15)
         error (ErrorError, "Cannot use R15 with %s", op);
     }
  if (!issmull && (issmlaxy || issmlawy))
    {
      if (inputLook () == ',')
        {
          inputSkip ();
          skipblanks ();
        }
      else
        error (ErrorError, "%sdst_l", InsertCommaAfter);
      dstl = getCpuReg ();
      if (dstl == 15)
        error (ErrorError, "Cannot use R15 with %s", op);
    }

  ir |= dstl << 12 | dsth << 16 | lhs | rhs << 8;
  putIns (ir);
}

void
m_smlabb (ARMWord cc)
{
  l_onlyregs (cc | M_SMLABB, "SMLABB");
}

void
m_smlabt (ARMWord cc)
{
  l_onlyregs (cc | M_SMLABT, "SMLABT");
}

void
m_smlatb (ARMWord cc)
{
  l_onlyregs (cc | M_SMLATB, "SMLATB");
}

void
m_smlatt (ARMWord cc)
{
  l_onlyregs (cc | M_SMLATT, "SMLATT");
}

void
m_smlalbb (ARMWord cc)
{
  l_onlyregs (cc | M_SMLALBB, "SMLALBB");
}

void
m_smlalbt (ARMWord cc)
{
  l_onlyregs (cc | M_SMLALBT, "SMLALBT");
}

void
m_smlaltb (ARMWord cc)
{
  l_onlyregs (cc | M_SMLALTB, "SMLALTB");
}

void
m_smlaltt (ARMWord cc)
{
  l_onlyregs (cc | M_SMLALTT, "SMLALTT");
}

void
m_smlawb (ARMWord cc)
{
  l_onlyregs (cc | M_SMLAWB, "SMLAWB");
}

void
m_smlawt (ARMWord cc)
{
  l_onlyregs (cc | M_SMLAWT, "SMLAWT");
}

void
m_smulbb (ARMWord cc)
{
  l_onlyregs (cc | M_SMULBB, "SMULBB");
}

void
m_smulbt (ARMWord cc)
{
  l_onlyregs (cc | M_SMULBT, "SMULBT");
}

void
m_smultb (ARMWord cc)
{
  l_onlyregs (cc | M_SMULTB, "SMULTB");
}

void
m_smultt (ARMWord cc)
{
  l_onlyregs (cc | M_SMULTT, "SMULTT");
}

void
m_smulwb (ARMWord cc)
{
  l_onlyregs (cc | M_SMULWB, "SMULWB");
}

void
m_smulwt (ARMWord cc)
{
  l_onlyregs (cc | M_SMULWT, "SMULWT");
}

void
m_smull (ARMWord cc)
{
  l_onlyregs (cc | M_SMULL, "SMULL");
}

void
m_smlal (ARMWord cc)
{
  l_onlyregs (cc | M_SMLAL, "SMLAL");
}

void
m_umull (ARMWord cc)
{
  l_onlyregs (cc | M_UMULL, "UMULL");
}

void
m_umlal (ARMWord cc)
{
  l_onlyregs (cc | M_UMLAL, "UMLAL");
}


void
m_clz (ARMWord cc)
{
  ARMWord ir = cc | M_CLZ;
  ARMWord dst, rhs;

  cpuWarn (XSCALE);

  dst = getCpuReg ();
  ir |= DST_OP (dst);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);

  rhs = getCpuReg ();
  ir |= RHS_OP (rhs);

  if (dst == 15 || rhs == 15)
    error(ErrorError, "Use of R15 in CLZ is unpredictable");

  putIns (ir);
}

static void
q_onlyregs (ARMWord ir, const char *op)
{
  ARMWord dst, lhs, rhs;

  cpuWarn (XSCALE);
  skipblanks ();

  dst = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  lhs = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  rhs = getCpuReg ();
  if (dst == 15 || lhs == 15 || rhs == 15)
    error (ErrorError, "Cannot use R15 with %s", op);

  ir |= dst << 12 | lhs | rhs << 16;
  putIns (ir);
}

void
m_qadd (ARMWord cc)
{
  q_onlyregs (cc | M_QADD, "QADD");
}

void
m_qdadd (ARMWord cc)
{
  q_onlyregs (cc | M_QDADD, "QDADD");
}

void
m_qdsub (ARMWord cc)
{
  q_onlyregs (cc | M_QDSUB, "QDSUB");
}

void
m_qsub (ARMWord cc)
{
  q_onlyregs (cc | M_QSUB, "QSUB");
}
