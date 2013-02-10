/*
 * AS an assembler for ARM
 * Copyright (c) 2012-2013 GCCSDK Developers
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
 * predef_reg.c
 */

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "main.h"
#include "predef_reg.h"
#include "symbol.h"

typedef struct
{
  const char *name;
  size_t len;
  int value;
  IntType_e type;
} PreDef_Reg_t;

/* These symbol registers are always defined.  */
static const PreDef_Reg_t oCPURegs[] =
{
  { "sp", sizeof ("sp")-1, 13, eIntType_CPU }, { "SP", sizeof ("SP")-1, 13, eIntType_CPU },
  { "lr", sizeof ("lr")-1, 14, eIntType_CPU }, { "LR", sizeof ("LR")-1, 14, eIntType_CPU },
  { "pc", sizeof ("pc")-1, 15, eIntType_CPU }, { "PC", sizeof ("PC")-1, 15, eIntType_CPU }
};

/* These symbol registers are always defined when APCS is selected.  */
static const PreDef_Reg_t oAPCSRegs[] =
{
  { "a1", sizeof ("a1")-1, 0, eIntType_CPU },
  { "a2", sizeof ("a2")-1, 1, eIntType_CPU },
  { "a3", sizeof ("a3")-1, 2, eIntType_CPU },
  { "a4", sizeof ("a4")-1, 3, eIntType_CPU },
  { "v1", sizeof ("v1")-1, 4, eIntType_CPU },
  { "v2", sizeof ("v2")-1, 5, eIntType_CPU },
  { "v3", sizeof ("v3")-1, 6, eIntType_CPU },
  { "v4", sizeof ("v4")-1, 7, eIntType_CPU },
  { "v5", sizeof ("v5")-1, 8, eIntType_CPU },
  { "ip", sizeof ("ip")-1, 12, eIntType_CPU }
};

static void
PreDefReg_One (const char *regname, size_t namelen, int value, IntType_e type)
{
  const Lex l = Lex_Id (regname, namelen);
  Symbol *symP = Symbol_Get (&l);
  const Value val = Value_Int (value, type);
  bool result = Symbol_Define (symP, SYMBOL_ABSOLUTE | SYMBOL_NO_EXPORT, &val);
  assert (!result);
}

/**
 * Defines a range of registers (lower & upper case version).
 * \param reg Lowercase register prefix.
 */
static void
PreDefReg_Range (char reg, unsigned num, IntType_e type, bool upcaseToo)
{
  for (unsigned regIdx = 0; regIdx != num; ++regIdx)
    {
      char regName[4];
      int numChars = sprintf (regName, "%c%d", reg, regIdx);
      PreDefReg_One (regName, numChars, regIdx, type);
      if (upcaseToo)
	{
	  regName[0] = reg + 'A' - 'a';
	  PreDefReg_One (regName, numChars, regIdx, type);
	}
    }
}


/**
 * Predefines registers based on REGNAMES, APCS and CPU/FPU/DEVICE options
 * at eStartUp phase time (depending on RegNames options).
 */
void
PreDefReg_PrepareForPhase (Phase_e phase)
{
  if (phase != eStartUp || gOptionRegNames == eNone)
    return;

  PreDefReg_Range ('r', 16, eIntType_CPU, true); /* CPU registers r0-r15, R0-R15.  */
  PreDefReg_Range ('p', 16, eIntType_CoProNum, false); /* Coprocessor numbers p0-p15.  */
  PreDefReg_Range ('c', 16, eIntType_CoProReg, false); /* Coprocessor registers c0-c15.  */
  /* FIXME: predefine acc0-acc7 ? */
  /* CPU registers sp, SP, lr, LR, pc and PC.  */
  for (size_t i = 0; i != sizeof (oCPURegs)/sizeof (oCPURegs[0]); ++i)
    PreDefReg_One (oCPURegs[i].name, oCPURegs[i].len, oCPURegs[i].value, oCPURegs[i].type);

  if (gOptionVersionAPCS != eAPCS_None || gOptionRegNames == eAll)
    {
      /* Define the base set of APCS registers.  */
      for (size_t i = 0; i != sizeof (oAPCSRegs)/sizeof (oAPCSRegs[0]); ++i)
	PreDefReg_One (oAPCSRegs[i].name, oAPCSRegs[i].len, oAPCSRegs[i].value, oAPCSRegs[i].type);

      /* IP */
      if (gOptionVersionAPCS == eAPCS_Empty || gOptionRegNames == eAll)
	PreDefReg_One ("IP", sizeof ("IP")-1, 12, eIntType_CPU);

      /* Depending on APCS flavour, define more APCS registers.  */
      /* sb/SB/v6 */
      if ((gOptionAPCS & APCS_OPT_REENTRANT) != 0 || gOptionRegNames == eAll)
	{
	  PreDefReg_One ("sb", sizeof ("sb")-1, 9, eIntType_CPU);
	  if (gOptionVersionAPCS == eAPCS_Empty || gOptionRegNames == eAll)
	    PreDefReg_One ("SB", sizeof ("SB")-1, 9, eIntType_CPU);
	}
      if ((gOptionAPCS & APCS_OPT_REENTRANT) != 0)
	{
	  if ((gOptionAPCS & APCS_OPT_SWSTACKCHECK) == 0 || gOptionRegNames == eAll)
	    PreDefReg_One ("v6", sizeof ("v6")-1, 10, eIntType_CPU);
	}
      else
	PreDefReg_One ("v6", sizeof ("v6")-1, 9, eIntType_CPU);

      /* sl/SL/v7 */
      if ((gOptionAPCS & APCS_OPT_SWSTACKCHECK) != 0 || gOptionRegNames == eAll)
	{
	  PreDefReg_One ("sl", sizeof ("sl")-1, 10, eIntType_CPU);
	  if (gOptionVersionAPCS == eAPCS_Empty || gOptionRegNames == eAll)
	    PreDefReg_One ("SL", sizeof ("SL")-1, 10, eIntType_CPU);
	}
      /* When gOptionRegNames is eAll and APCS_OPT_REENTRANT, this makes that
         both v7 and v6 are defined but v6 = 10 and v7 = 9 ! */
      if ((gOptionAPCS & (APCS_OPT_REENTRANT | APCS_OPT_SWSTACKCHECK)) == 0
          || ((gOptionAPCS & APCS_OPT_REENTRANT) == 0 && gOptionRegNames == eAll))
	PreDefReg_One ("v7", sizeof ("v7")-1, 10, eIntType_CPU);
      else if ((gOptionAPCS & APCS_OPT_REENTRANT) != 0 && gOptionRegNames == eAll)
	PreDefReg_One ("v7", sizeof ("v7")-1, 9, eIntType_CPU);

      /* fp/FP/v8 */
      /* When no framepointer is selected, ObjAsm will define fp and/or FP and
         warn when they are used.  AsAsm doesn't define them at all.  */
      if ((gOptionAPCS & APCS_OPT_FRAMEPTR) != 0 || gOptionRegNames == eAll)
	{
	  PreDefReg_One ("fp", sizeof ("fp")-1, 11, eIntType_CPU);
	  if (gOptionVersionAPCS == eAPCS_Empty || gOptionRegNames == eAll)
	    PreDefReg_One ("FP", sizeof ("FP")-1, 11, eIntType_CPU);
	}
      if ((gOptionAPCS & APCS_OPT_FRAMEPTR) == 0 || gOptionRegNames == eAll)
	PreDefReg_One ("v8", sizeof ("v8")-1, 11, eIntType_CPU);
    }

  if (1 /* FIXME: only when FPA is selected.  */ || gOptionRegNames == eAll)
    PreDefReg_Range ('f', 8, eIntType_FPU, true);

  /* FIXME: define single/double precision VFP registers based on chosen arch/fpu etc.
  if (0 ||  || gOptionRegNames == eAll)
  PreDefReg_Range ('q', 16, eIntType_NeonQuadReg, true);
  PreDefReg_Range ('d', 32, eIntType_NeonOrVFPDoubleReg, true);
  PreDefReg_Range ('s', 32, eIntType_VFPSingleReg, true); */
}
