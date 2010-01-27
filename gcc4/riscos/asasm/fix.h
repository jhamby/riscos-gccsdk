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
 * fix.h
 */

#ifndef fix_header_included
#define fix_header_included

#include "global.h"

#define RRX 5
#define LSL 0x00000000
#define ASL 0x00000000
#define LSR 0x00000020
#define ASR 0x00000040
#define ROR 0x00000060
#define NO_SHIFT 0

#define SHIFT_OP(s) ((s)==RRX?ROR:s)	/* RRX coded as ROR 0 */
#define SHIFT_REG(s) (((s)<<8) | (1<<4))
#define SHIFT_IMM(s) (((s)&31)<<7)

WORD fixShiftImm (int lineno, WORD shiftop, int shift);
WORD fixImm8s4 (int lineno, WORD ir, int im);
WORD fixImmFloat (int lineno, WORD ir, FLOAT im);
WORD fixSwi (int lineno, int im);
WORD fixBranch (int lineno, int im);
WORD fixBranchT (int lineno, int im);
WORD fixAdr (int lineno, WORD ir, int im);
void fixAdrl (int lineno, WORD * ir, WORD * ir2, int im, int warn);
WORD fixCopOffset (int lineno, WORD ir, int offset);
WORD fixCpuOffset (int lineno, WORD ir, int offset);
WORD fixMask (int lineno, int mask);
WORD fixInt (int lineno, int size, int value);

#endif
