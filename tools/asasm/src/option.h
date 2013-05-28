/*
 * AsAsm an assembler for ARM
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef option_header_included
#define option_header_included

#include <stdbool.h>

#include "global.h"

ARMWord Option_Cond (bool doLowerCase);
ARMWord Option_CondS (bool doLowerCase);
ARMWord Option_SCond (bool doLowerCase);
ARMWord Option_CondSP (bool doLowerCase);
ARMWord Option_CondB (bool doLowerCase);
ARMWord Option_LdrStrCondAndType (bool isStore, bool doLowerCase);
ARMWord Option_CondRfeSrs (bool isLoad, bool doLowerCase);
ARMWord Option_CondLdmStm (bool isLDM, bool doLowerCase);
ARMWord Option_CondLfmSfm (bool doLowerCase);
ARMWord Option_CondPrecRound (bool doLowerCase);
ARMWord Option_CondPrec_P (bool doLowerCase);
ARMWord Option_CondL (bool doLowerCase);

ARMWord Option_CondOptRound (bool doLowerCase);
ARMWord Option_LinkCond (bool doLowerCase);
ARMWord Option_ExceptionCond (bool doLowerCase);
ARMWord Option_ADRL (bool doLowerCase);

ARMWord Option_GetCCodeIfThere (bool doLowerCase);

#define kOption_NotRecognized ((ARMWord) -1)

typedef enum
{
  eInstrWidth_Unrecognized,
  eInstrWidth_NotSpecified,
  eInstrWidth_Enforce16bit, /* .N is specified. Thumb code: enforce 16bit (when
    not possible -> error).  ARM code : error.  */
  eInstrWidth_Enforce32bit, /* .W is specified. Thumb code (ARMv6T2 or later) :
    force 32bit thumb variant.  ARM code : ignore.  */
} InstrWidth_e;

InstrWidth_e Option_GetInstrWidth (bool doLowerCase);

#define EQ ((ARMWord)0u<<28)
#define NE ((ARMWord)1u<<28)
#define CS ((ARMWord)2u<<28)
#define CC ((ARMWord)3u<<28)
#define MI ((ARMWord)4u<<28)
#define PL ((ARMWord)5u<<28)
#define VS ((ARMWord)6u<<28)
#define VC ((ARMWord)7u<<28)
#define HI ((ARMWord)8u<<28)
#define LS ((ARMWord)9u<<28)
#define GE ((ARMWord)10u<<28)
#define LT ((ARMWord)11u<<28)
#define GT ((ARMWord)12u<<28)
#define LE ((ARMWord)13u<<28)
#define AL ((ARMWord)14u<<28)
#define NV ((ARMWord)15u<<28)

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
#define EXCEPTION_BIT	((ARMWord)0x00400000)

#define STACKMODE_IB    (P_FLAG | U_FLAG)
#define STACKMODE_IA    (         U_FLAG)
#define STACKMODE_DB    (P_FLAG |      0)
#define STACKMODE_DA    (     0 |      0)
#define STACKMODE_BITS  (P_FLAG | U_FLAG)

/* FPA precision bits (except for LDF/STF usage).  */
#define PRECISION_SINGLE    ((ARMWord)0x00000000)
#define PRECISION_DOUBLE    ((ARMWord)0x00000080)
#define PRECISION_EXTENDED  ((ARMWord)0x00080000)
#define PRECISION_MASK      ((ARMWord)0x00080080)

/* FPA precision bits for LDF/STF usage.  */
#define PRECISION_MEM_SINGLE    ((ARMWord)0x00000000)
#define PRECISION_MEM_DOUBLE    ((ARMWord)0x00008000)
#define PRECISION_MEM_EXTENDED  ((ARMWord)0x00400000)
#define PRECISION_MEM_PACKED    ((ARMWord)0x00408000)
#define PRECISION_MEM_MASK      ((ARMWord)0x00408000)

#define ROUND_NEAREST       ((ARMWord)0x00)
#define ROUND_PLUSINF       ((ARMWord)0x20)
#define ROUND_MINUSINF      ((ARMWord)0x40)
#define ROUND_ZERO          ((ARMWord)0x60)

#define FORCE_FLAG	((ARMWord)1<<22)

#define ARM_MODE_USR	(0x10)
#define ARM_MODE_FIQ	(0x11)
#define ARM_MODE_IRQ	(0x12)
#define ARM_MODE_SVC	(0x13)
#define ARM_MODE_ABORT	(0x17)
#define ARM_MODE_UNDEF	(0x1B)
#define ARM_MODE_SYSTEM	(0x1F)

bool Option_IsValidARMMode (int armMode);

#endif
