/*
 * AS an assembler for ARM
 * Copyright (c) 2012 GCCSDK Developers
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
  int type; /* SYMBOL_CPUREG, etc.  */
} PreDef_Reg_t;

/* These symbol registers are always defined.  */
static const PreDef_Reg_t oCPURegs[] =
{
  { "lr", sizeof ("lr")-1, 14, SYMBOL_CPUREG }, { "LR", sizeof ("LR")-1, 14, SYMBOL_CPUREG },
  { "pc", sizeof ("pc")-1, 15, SYMBOL_CPUREG }, { "PC", sizeof ("PC")-1, 15, SYMBOL_CPUREG },
};

/* These symbol registers are always defined when APCS is selected.  */
static const PreDef_Reg_t oAPCSRegs[] =
{
  { "a1", sizeof ("a1")-1, 0, SYMBOL_CPUREG },
  { "a2", sizeof ("a2")-1, 1, SYMBOL_CPUREG },
  { "a3", sizeof ("a3")-1, 2, SYMBOL_CPUREG },
  { "a4", sizeof ("a4")-1, 3, SYMBOL_CPUREG },
  { "v1", sizeof ("v1")-1, 4, SYMBOL_CPUREG },
  { "v2", sizeof ("v2")-1, 5, SYMBOL_CPUREG },
  { "v3", sizeof ("v3")-1, 6, SYMBOL_CPUREG },
  { "v4", sizeof ("v4")-1, 7, SYMBOL_CPUREG },
  { "v5", sizeof ("v5")-1, 8, SYMBOL_CPUREG },
  /* Note: v6, sl and fp are conditionally defined depending on the APCS option value.  */
  { "ip", sizeof ("ip")-1, 12, SYMBOL_CPUREG }, /* { "IP", sizeof ("IP")-1, 12, SYMBOL_CPUREG }, */
  { "sp", sizeof ("sp")-1, 13, SYMBOL_CPUREG }, /* { "SP", sizeof ("SP")-1, 13, SYMBOL_CPUREG }, */
};

static void
PreDefReg_One (const char *regname, size_t namelen, int value, int type)
{
  const Lex l = lexTempLabel (regname, namelen);
  Symbol *s = symbolGet (&l);
  s->type |= SYMBOL_DEFINED | SYMBOL_ABSOLUTE | type;
  s->value = Value_Int (value);
}

/**
 * Defines a range of registers (lower & upper case version).
 * \param reg Lowercase register prefix.
 */
static void
PreDefReg_Range (char reg, unsigned num, int type, bool upcaseToo)
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

void
PreDefReg_Init (void)
{
  PreDefReg_Range ('r', 16, SYMBOL_CPUREG, true);
  PreDefReg_Range ('p', 16, SYMBOL_COPNUM, false); /* Coprocessor numbers.  */
  PreDefReg_Range ('c', 16, SYMBOL_COPREG, false); /* Coprocessor registers.  */
  for (size_t i = 0; i != sizeof (oCPURegs)/sizeof (oCPURegs[0]); ++i)
    PreDefReg_One (oCPURegs[i].name, oCPURegs[i].len, oCPURegs[i].value, oCPURegs[i].type);

  if (gIsAPCS)
    {
      for (size_t i = 0; i != sizeof (oAPCSRegs)/sizeof (oAPCSRegs[0]); ++i)
	PreDefReg_One (oAPCSRegs[i].name, oAPCSRegs[i].len, oAPCSRegs[i].value, oAPCSRegs[i].type);
      PreDefReg_One ((gOptionAPCS & APCS_OPT_REENTRANT) ? "sb" : "v6",
		     sizeof ("sb")-1, 9, SYMBOL_CPUREG);
      PreDefReg_One ((gOptionAPCS & APCS_OPT_SWSTACKCHECK) ? "sl" : "v7",
		     sizeof ("sl")-1, 10, SYMBOL_CPUREG);
      PreDefReg_One ((gOptionAPCS & APCS_OPT_FRAMEPTR) ? "fp" : "v8",
		     sizeof ("fp")-1, 11, SYMBOL_CPUREG);
    }

  if (1 /* FIXME: only when FPA is selected.  */)
    PreDefReg_Range ('f', 8, SYMBOL_FPUREG, true);

  /* FIXME: define single/double precision VFP registers based on chosen arch/fpu etc.
  PreDefReg_Range ('q', 16, SYMBOL_NEONQUADREG, true);
  PreDefReg_Range ('d', 32, SYMBOL_NEONDOUBLEREG, true);
  PreDefReg_Range ('s', 32, SYMBOL_VFPSINGLEREG, true); */
}
