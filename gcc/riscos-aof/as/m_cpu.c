
/*
 * m_cpu.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "mnemonics.h"
#include "error.h"
#include "option.h"
#include "put.h"
#include "input.h"
#include "global.h"
#include "expr.h"
#include "area.h"
#include "get.h"
#include "m_cpu.h"
#include "targetcpu.h"

/** DATA none (or optional register) **/

void 
m_nop (void)
{
  if (!inputComment ())
    {
      WORD op = getCpuReg ();
      if (op == 15)
	error (ErrorError, TRUE, "Cannot use R15 in NOP");
      putIns (0xE1A00000 | DST_OP (op) | RHS_OP (op));
    }
  else
    putIns (0xE1A00000);
}

/** DATA dst=lhs<op>rhs **/

static void 
dstlhsrhs (WORD ir)
{
  WORD op;
  op = getCpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst", InsertCommaAfter);
  op = getCpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  ir = getRhs (FALSE, TRUE, ir);
  putIns (ir);
}

static void 
dstrhs (WORD ir)
{
  WORD op;
  op = getCpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst", InsertCommaAfter);
  ir = getRhs (FALSE, TRUE, ir);
  putIns (ir);
}

void 
m_adc (WORD cc)
{
  dstlhsrhs (cc | M_ADC);
}

void 
m_add (WORD cc)
{
  dstlhsrhs (cc | M_ADD);
}

void 
m_and (WORD cc)
{
  dstlhsrhs (cc | M_AND);
}

void 
m_bic (WORD cc)
{
  dstlhsrhs (cc | M_BIC);
}

void 
m_eor (WORD cc)
{
  dstlhsrhs (cc | M_EOR);
}

void 
m_mov (WORD cc)
{
  dstrhs (cc | M_MOV);
}

void 
m_mvn (WORD cc)
{
  dstrhs (cc | M_MVN);
}

void 
m_orr (WORD cc)
{
  dstlhsrhs (cc | M_ORR);
}

void 
m_rsb (WORD cc)
{
  dstlhsrhs (cc | M_RSB);
}

void 
m_rsc (WORD cc)
{
  dstlhsrhs (cc | M_RSC);
}

void 
m_sbc (WORD cc)
{
  dstlhsrhs (cc | M_SBC);
}

void 
m_sub (WORD cc)
{
  dstlhsrhs (cc | M_SUB);
}

/** DATA test **/

static void 
lhsrhs (WORD ir)
{
  WORD op;
  op = getCpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  ir = getRhs (FALSE, TRUE, ir);
  putIns (ir);
}

void 
m_cmn (WORD cc)
{
  lhsrhs (cc | M_CMN);
}

void 
m_cmp (WORD cc)
{
  lhsrhs (cc | M_CMP);
}

void 
m_teq (WORD cc)
{
  lhsrhs (cc | M_TEQ);
}

void 
m_tst (WORD cc)
{
  lhsrhs (cc | M_TST);
}

/** DATA 1a **/

extern int fussy;

static void 
onlyregs (BOOL acc, WORD ir)
{
  WORD dst, rhs, lhs;
  dst = getCpuReg ();
  ir |= DST_MUL (dst);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst", InsertCommaAfter);
  lhs = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  rhs = getCpuReg ();
  if (dst == lhs)
    {
      if (dst == rhs)
	error (ErrorError, TRUE, "Destination and left operand are the same register %d", dst);
      else
	{
	  if (fussy)
	    error (ErrorInfo, TRUE, "Changing order of operands in %s", acc ? "MLA" : "MUL");
	  {
	    int t = lhs;
	    lhs = rhs;
	    rhs = t;
	  }
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
	error (ErrorError, TRUE, "%srhs", InsertCommaAfter);
      ir |= ACC_MUL (getCpuReg ());
      skipblanks ();
    }
  putIns (ir);
}

void 
m_mla (WORD cc)
{
  onlyregs (TRUE, cc | M_MLA);
}

void 
m_mul (WORD cc)
{
  onlyregs (FALSE, cc | M_MUL);
}

static void 
l_onlyregs (WORD ir, const char *op)
{
  WORD dstl, dsth, lhs, rhs;
  cpuWarn (ARM7M);
  skipblanks ();
  dstl = getCpuReg ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst_h", InsertCommaAfter);
  dsth = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst_l", InsertCommaAfter);
  lhs = getCpuReg ();
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  rhs = getCpuReg ();
  if (dstl == dsth)
    error (ErrorError, TRUE, "Destination high and low are the same register %d", dstl);
  if (dstl == lhs || dsth == lhs)
    {
      if (dstl == rhs || dsth == rhs)
	error (ErrorError, TRUE, "Left operand register %d also occurs in destination", lhs);
      else
	{
	  if (fussy)
	    error (ErrorInfo, TRUE, "Changing order of operands in %s", op);
	  {
	    int t = lhs;
	    lhs = rhs;
	    rhs = t;
	  }
	}
    }
  ir |= dstl << 12 | dsth << 16 | lhs | rhs << 8;
  putIns (ir);
}

void 
m_smull (WORD cc)
{
  l_onlyregs (cc | M_SMULL, "SMULL");
}

void 
m_smlal (WORD cc)
{
  l_onlyregs (cc | M_SMLAL, "SMLAL");
}

void 
m_umull (WORD cc)
{
  l_onlyregs (cc | M_UMULL, "UMULL");
}

void 
m_umlal (WORD cc)
{
  l_onlyregs (cc | M_UMLAL, "UMLAL");
}
