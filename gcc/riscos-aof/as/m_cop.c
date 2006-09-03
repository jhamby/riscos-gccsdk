/*
 * AS an assembler for ARM
 * Copyright © 1992 Niklas Röjemo
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
 * m_cop.c
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
#include "help_cop.h"
#include "input.h"
#include "main.h"
#include "mnemonics.h"
#include "option.h"
#include "put.h"
#include "reloc.h"
#include "targetcpu.h"

static void
coprocessor (BOOL CopOnly, int ir, int maxop)	/* p#,cpop,cpdst,cplhs,cprhs {,info} */
{
  int cop = CP_NUMBER (getCopNum ());

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
m_cdp (WORD cc)			/* cdp CC p#,cpop,cpdst,cplhs,cprhs {,info} */
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
