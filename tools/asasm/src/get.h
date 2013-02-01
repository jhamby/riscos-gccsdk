/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2013 GCCSDK Developers
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
 * get.h
 */

#ifndef get_header_included
#define get_header_included

#include <stdbool.h>

#include "global.h"

unsigned Get_CPUReg (void);
unsigned Get_CPURegNoError (void);
unsigned Get_FPUReg (void);
unsigned Get_CopReg (void);
unsigned Get_CopNum (void);
unsigned Get_CPURList (void);
ARMWord Get_RHS (bool immonly, bool shift, ARMWord ir);

#define DST_OP(op) ((op)<<12)
#define LHS_OP(op) ((op)<<16)
#define RHS_OP(op) ( op     )

#define GET_DST_OP(instr)  (((instr)>>12) & 15)

#define DST_MUL(r) ((r)<<16)
#define LHS_MUL(r) ( r     )
#define RHS_MUL(r) ((r)<< 8)
#define ACC_MUL(r) ((r)<<12)

#define IMM_RHS   0x02000000

#define INVALID_REG 0xFFFFFFFFu

#define RRX 5
#define LSL 0x00000000
#define ASL LSL
#define LSR 0x00000020
#define ASR 0x00000040
#define ROR 0x00000060
#define NO_SHIFT 0

#define SHIFT_OP(s) ((s) == RRX ? ROR : (s))	/* RRX coded as ROR 0 */
#define SHIFT_REG(s) (((s)<<8) | (1<<4))
#define SHIFT_IMM(s) (((s)&31)<<7)

#endif
