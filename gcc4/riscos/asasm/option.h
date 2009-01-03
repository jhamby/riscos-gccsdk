/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2006 GCCSDK Developers
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

#include "global.h"

WORD optionCond (void);
WORD optionCondS (void);
WORD optionCondSP (void);
WORD optionCondB (void);
WORD optionCondBT (void);
WORD optionCondDirLdm (void);
WORD optionCondDirStm (void);
WORD optionCondLfmSfm (void);
WORD optionCondPrecRound (void);
WORD optionCondOptPrecRound (void);
WORD optionCondPrec_P (void);
WORD optionCondL (void);

WORD optionCondOptRound (void);
WORD optionLinkCond (void);
WORD optionExceptionCond (void);
WORD optionAdrL (void);

#define optionError ((WORD) -1)

#define EQ  ((WORD)0<<28)
#define NE  ((WORD)1<<28)
#define CS  ((WORD)2<<28)
#define CC  ((WORD)3<<28)
#define MI  ((WORD)4<<28)
#define PL  ((WORD)5<<28)
#define VS  ((WORD)6<<28)
#define VC  ((WORD)7<<28)
#define HI  ((WORD)8<<28)
#define LS  ((WORD)9<<28)
#define GE ((WORD)10<<28)
#define LT ((WORD)11<<28)
#define GT ((WORD)12<<28)
#define LE ((WORD)13<<28)
#define AL ((WORD)14<<28)
#define NV ((WORD)15<<28)

#define HS CS
#define LO CC

#define S_FLAG ((WORD) 1<<20)

#define T_FLAG ((WORD) 1<<21)
#define B_FLAG ((WORD) 1<<22)
#define P_FLAG ((WORD)15<<12)
#define L_FLAG B_FLAG

#define LINK_BIT     ((WORD)1<<24)
#define EXEPTION_BIT ((WORD)0x00400000)

#define IB ((WORD)0x01800000)
#define IA ((WORD)0x00800000)
#define DB ((WORD)0x01000000)
#define DA ((WORD)0x00000000)

#define PRECISION_SINGLE    ((WORD)0x00000000)
#define PRECISION_DOUBLE    ((WORD)0x00000080)
#define PRECISION_EXTENDED  ((WORD)0x00080000)

#define PRECISION_MEM_SINGLE    ((WORD)0x00000000)
#define PRECISION_MEM_DOUBLE    ((WORD)0x00008000)
#define PRECISION_MEM_EXTENDED  ((WORD)0x00400000)
#define PRECISION_MEM_PACKED    ((WORD)0x00408000)

#define ROUND_NEAREST       ((WORD)0x00)
#define ROUND_PLUSINF       ((WORD)0x20)
#define ROUND_MINUSINF      ((WORD)0x40)
#define ROUND_ZERO          ((WORD)0x60)


#define REG_FLAG ((WORD)1<<25)
#define PRE_FLAG ((WORD)1<<24)
#define UP_FLAG  ((WORD)1<<23)

#define WB_FLAG  T_FLAG

#define FORCE_FLAG ((WORD)1<<22)


#endif
