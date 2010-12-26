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
 * option.h
 */

#ifndef option_header_included
#define option_header_included

#include <stdbool.h>

#include "global.h"

ARMWord optionCond (void);
ARMWord optionCondS (void);
ARMWord optionCondSP (void);
ARMWord optionCondB (void);
ARMWord optionCondBT (bool isStore);
ARMWord optionCondLdmStm (bool isLDLM);
ARMWord optionCondLfmSfm (void);
ARMWord optionCondPrecRound (void);
ARMWord optionCondOptPrecRound (void);
ARMWord optionCondPrec_P (void);
ARMWord optionCondL (void);

ARMWord optionCondOptRound (void);
ARMWord optionLinkCond (void);
ARMWord optionExceptionCond (void);
ARMWord optionAdrL (void);

#define optionError ((ARMWord) -1)

#define EQ ((ARMWord)0<<28)
#define NE ((ARMWord)1<<28)
#define CS ((ARMWord)2<<28)
#define CC ((ARMWord)3<<28)
#define MI ((ARMWord)4<<28)
#define PL ((ARMWord)5<<28)
#define VS ((ARMWord)6<<28)
#define VC ((ARMWord)7<<28)
#define HI ((ARMWord)8<<28)
#define LS ((ARMWord)9<<28)
#define GE ((ARMWord)10<<28)
#define LT ((ARMWord)11<<28)
#define GT ((ARMWord)12<<28)
#define LE ((ARMWord)13<<28)
#define AL ((ARMWord)14<<28)
#define NV ((ARMWord)15<<28)

#define HS CS
#define LO CC

/* Address mode flags (LDR* and STR*): */

/* Address mode 3 only.  */
#define H_FLAG		((ARMWord)1<<5)
/* Address mode 3 only.  */
#define S_FLAG		((ARMWord)1<<6)
#define L_FLAG		((ARMWord)1<<20)
/* For the 'T' in LDRBT, LDRT, STRBT or STRT and address writeback into base
   register.  */
#define W_FLAG		((ARMWord)1<<21)
/* Byte or word access for LDR/STR (address mode 2 only).  */
#define B_FLAG		((ARMWord)1<<22)
/* Add or subtract offset to base.  */
#define U_FLAG		((ARMWord)1<<23)
/* offset addressing or pre-indexed addressing instead of post-indexed
   addressing  */
#define P_FLAG		((ARMWord)1<<24)
/* Use (scaled) register offset/index instead of immediate offset/index.  */
#define REG_FLAG	((ARMWord)1<<25)

/* The 'S' flag to set the PSR bits.  */
#define PSR_S_FLAG	((ARMWord) 1<<20)

/* For use in TST, TEQ, CMN and CMP.  */
#define PSR_P_FLAG	((ARMWord)15<<12)

/* For use in LDC/LDC2.  */
#define N_FLAG		((ARMWord)1<<22)

#define LINK_BIT	((ARMWord)1<<24)
#define EXEPTION_BIT	((ARMWord)0x00400000)

#define IB ((ARMWord)0x01800000)
#define IA ((ARMWord)0x00800000)
#define DB ((ARMWord)0x01000000)
#define DA ((ARMWord)0x00000000)

#define PRECISION_SINGLE    ((ARMWord)0x00000000)
#define PRECISION_DOUBLE    ((ARMWord)0x00000080)
#define PRECISION_EXTENDED  ((ARMWord)0x00080000)

#define PRECISION_MEM_SINGLE    ((ARMWord)0x00000000)
#define PRECISION_MEM_DOUBLE    ((ARMWord)0x00008000)
#define PRECISION_MEM_EXTENDED  ((ARMWord)0x00400000)
#define PRECISION_MEM_PACKED    ((ARMWord)0x00408000)

#define ROUND_NEAREST       ((ARMWord)0x00)
#define ROUND_PLUSINF       ((ARMWord)0x20)
#define ROUND_MINUSINF      ((ARMWord)0x40)
#define ROUND_ZERO          ((ARMWord)0x60)

#define FORCE_FLAG	((ARMWord)1<<22)

#endif
