
/*
 * m_cop.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <stdint.h>
#include "mnemonics.h"
#include "error.h"
#include "option.h"
#include "input.h"
#include "expr.h"
#include "reloc.h"
#include "put.h"
#include "area.h"
#include "get.h"
#include "help_cop.h"
#include "targetcpu.h"

extern int pedantic;

static void 
coprocessor (BOOL CopOnly, int ir, int maxop)	/* cp#,cpop,cpdst,cplhs,cprhs {,info} */
{
  int cop;

  cop = CP_NUMBER (getCopNum ());

  /* int cop = CP_NUMBER(help_copInt(15,"coprocessor number")); */
  if (cop == 1)
    {
      if (pedantic)
	error (ErrorInfo, TRUE, "Coprocessor 1 is the floating point unit. Use a floating point mnemonic if possible");
    }
  else
    cpuWarn (ARM3);
  ir |= cop;
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%scoprocessor number", InsertCommaAfter);
  if (maxop > 7)
    ir |= CP_DCODE (help_copInt (maxop, "coprocessor opcode"));
  else
    ir |= CP_RTRAN (help_copInt (maxop, "coprocessor opcode"));
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdata operand", InsertCommaAfter);
  ir |= CopOnly ? CPDST_OP (getCopReg ()) : CPDST_OP (getCpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%sdst", InsertCommaAfter);
  ir |= CPLHS_OP (getCopReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%slhs", InsertCommaAfter);
  ir |= CPRHS_OP (getCopReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
      ir |= CP_INFO (help_copInt (7, "coprocessor info"));
    }
  putIns (ir);
}

void 
m_cdp (WORD cc)			/* cdp CC cp#,cpop,cpdst,cplhs,cprhs {,info} */
{
  coprocessor (TRUE, cc | 0x0e000000, 15);
}

/** REGISTER TRANSFER **/

void 
m_mcr (WORD cc)
{
  coprocessor (FALSE, cc | 0x0e000010, 15);
}

void 
m_mrc (WORD cc)
{
  coprocessor (FALSE, cc | 0x0e100010, 15);
}
