/*
 * AS an assembler for ARM
 * Copyright (c) 2010-2012 GCCSDK Developers
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
 * targetcpu.h
 */

#ifndef targetcpu_header_included
#define targetcpu_header_included

#include <stdbool.h>

/* ARM architectures:
   (keep this in sync with oARMArch @ targetcpu.c) */
typedef enum
{
  ARCH_ARMv1 = 0,
  ARCH_ARMv2,		/* MUL, MLA */
  ARCH_ARMv2a,		/* SWP */
  ARCH_ARMv3,
  ARCH_ARMv3M,
  ARCH_ARMv4,
  ARCH_ARMv4T,		/* Thumb. */
  ARCH_ARMv5,
  ARCH_ARMv5T = ARCH_ARMv5,	/* CLZ, BKPT */
  ARCH_ARMv5TE,		/* DSP algorithms, PLD, LDRD/STRD, MCRR, MRRC, Jazelle, VFPv2 opt */
  ARCH_ARMv5TEJ,	/* BXJ, Jazelle, VFPv2 opt */
  ARCH_ARMv6,		/* Jazelle, VFPv2 opt */
  ARCH_ARMv6J,		/* Jazelle */
  ARCH_ARMv6K,		/* Jazelle, security opt */
  ARCH_ARMv6KZ,		/* Jazelle */
  ARCH_ARMv6Z,		/* Jazelle */
  ARCH_ARMv6M,		/* Jazelle */
  ARCH_ARMv6T2,		/* Jazelle, Thumb-2, VFPv2 opt */
  ARCH_ARMv7,
  ARCH_ARMv7A = ARCH_ARMv7,		/* Jazelle, Thumb-2EE (Thumb-2 Execution Environment), multiprocessor opt, VFPv3 opt, SIMD opt, security opt */
  ARCH_ARMv7R,		/* Jazelle, Thumb-2EE, multiprocessor opt, VFPv3 opt, SIMD opt */
  ARCH_ARMv7EM,		/* Jazelle, Thumb-2EE */
  ARCH_ARMv7M		/* Jazelle, Thumb-2EE */
} ARM_eArchitectures;

typedef enum
{
  eFeature_Thumb,
  eFeature_Thumb2
} Arch_Feature_e;

bool Target_NeedAtLeastArch (ARM_eArchitectures arch);

bool Target_SetCPU (const char *cpu);

const char *Target_GetCPU (void);
const char *Target_GetArchAsString (void);
ARM_eArchitectures Target_GetArch (void);

void Target_CheckFeature (Arch_Feature_e feature);

#endif
