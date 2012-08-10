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
 * targetcpu.c
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "error.h"
#include "targetcpu.h"

typedef struct CPUArch_ListEntry_s CPUArch_ListEntry_t;
struct CPUArch_ListEntry_s
{
  const char *cpu_arch; /** Architecture name (when archP is NULL), CPU name
    (when archP is non-NULL).  */
  const CPUArch_ListEntry_t *archP; /** NULL for architecture, non-NULL when
    pointed to its architecture.  */
  unsigned cpu_features; /** For architectures, this set will be the architecture
    features.  For CPUs, this will be the extra features compared to the
    architecture pointed via archP.  */
  unsigned fpu_features; /** FPU hardware capabilities.  */
};

typedef struct
{
  const char *fpu;
  unsigned features;
} FPU_ListEnty_t;

typedef struct
{
  const char *device;
  const char *core;
  const char *fpu;
} Device_ListEntry_t;

static const CPUArch_ListEntry_t *oTarget_CPU;
static unsigned oTarget_CPUFeatures;
static const FPU_ListEnty_t *oTarget_FPU;
static unsigned oTarget_FPUFeatures;
static const Device_ListEntry_t *oTarget_Device;

typedef enum
{
  eArch_1 = 0,
  eArch_2,
  eArch_2a,
  eArch_3,
  eArch_3G,
  eArch_3M,
  eArch_4,
  eArch_4xM,
  eArch_4T,
  eArch_4TxM,
  eArch_5,
  eArch_5xM,
  eArch_5T,
  eArch_5TxM,
  eArch_5TE,
  eArch_5TEJ,
  eArch_5TExP,
  eArch_6,
  eArch_6K,
  eArch_6KZ,
  eArch_6Z,
  eArch_6T2,
  eArch_6M,
  eArch_6SM, 
  eArch_7,
  eArch_7A,
  eArch_7A_security,
  eArch_7R,
  eArch_7EM,
  eArch_7M
} Arch_e;

/* Maps ARM cores to architectures (including architectures themselves): */
static const CPUArch_ListEntry_t oCPUArchList[] =
{
  /* Pure architectures:
   * When adding or removing architectures, don't forget to adjust Arch_e !
   */
  { "1", NULL, kArchExt_v1, kArchFPUExt_None },
  { "2", NULL, kArchExt_v2, kArchFPUExt_None }, /* MUL/MLA.  */
  { "2a", NULL, kArchExt_v2a, kArchFPUExt_None }, /* On-chip cache, SWP[B].  */
  { "3", NULL, kArchExt_v3, kArchFPUExt_None }, /* ARMv3 without long multiply.  */
  { "3G", NULL, kArchExt_v3, kArchFPUExt_None },
  { "3M", NULL, kArchExt_v3M, kArchFPUExt_None }, /* ARMv3 with long multiply.  */
  { "4", NULL, kArchExt_v4, kArchFPUExt_None }, /* ARMv4 with long multiply but no Thumb.  */
  { "4xM", NULL, kArchExt_v4xM, kArchFPUExt_None }, /* ARMv4 without long multiply or Thumb.  */
  { "4T", NULL, kArchExt_v4T, kArchFPUExt_None }, /* ARMv4 with long multiply and Thumb.  */
  { "4TxM", NULL, kArchExt_v4TxM, kArchFPUExt_None }, /* ARMv4 without long multiply but with Thumb.  */
  { "5", NULL, kArchExt_v5, kArchFPUExt_None }, /* ARMv5 with long multiply.  */
  { "5xM", NULL, kArchExt_v5xM, kArchFPUExt_None }, /* ARMv5 without long multiply.  */
  { "5T", NULL, kArchExt_v5T, kArchFPUExt_None }, /* ARMv5 with long multiply, Thumb and interworking.  */
  { "5TxM", NULL, kArchExt_v5TxM, kArchFPUExt_None }, /* ARMv5 without long multiply but with Thumb and interworking.  */
  { "5TE", NULL, kArchExt_v5TE, kArchFPUExt_None }, /* ARMv5 with long multiply, Thumb, DSP multiply, and double-word instructions.  */ 
  { "5TEJ", NULL, kArchExt_v5TEJ, kArchFPUExt_None }, /* ARMv5 with long multiply, Thumb, DSP multiply, double-word instructions, and Jazelle extensions.  */
  { "5TExP", NULL, kArchExt_v5TExP, kArchFPUExt_None }, /* ARMv5 with long multiply, Thumb, DSP multiply.  */
  { "6", NULL, kArchExt_v6, kArchFPUExt_None }, /* ARMv6 with Thumb, interworking, DSP multiply, double-word instructions, unaligned and mixed-endian support, Jazelle, and media extensions.  */
  { "6K", NULL, kArchExt_v6K, kArchFPUExt_None }, /* ARMv6 with SMP/kernel extensions. MPCore */
  { "6KZ", NULL, kArchExt_v6KZ, kArchFPUExt_None }, /* AsAsm extension.  */ /* ARMv6KZ & ARMv6Z contain the optional Security Extensions.  */
  { "6Z", NULL, kArchExt_v6Z, kArchFPUExt_None }, /* ARMv6 with Security/TrustZone Extensions (SMI).  */
  { "6T2", NULL, kArchExt_v6T2, kArchFPUExt_None }, /* ARMv6 with Thumb-2.  */
  { "6-M", NULL, kArchExt_v6M, kArchFPUExt_None }, /* ARMv6 micro-controller profile with Thumb only (not Thumb-2), plus processor state instructions.  */
  { "6S-M", NULL, kArchExt_v6SM, kArchFPUExt_None }, /* ARMv6 micro-controller profile with Thumb only (not Thumb-2), plus processor state instructions and OS extensions.  */ 
  { "7", NULL, kArchExt_v7, kArchFPUExt_None }, /* ARMv7 with Thumb (Thumb-2 technology) only, and without hardware divide.  */
  { "7-A", NULL, kArchExt_v7A, kArchFPUExt_None }, /* ARMv7 application profile supporting virtual MMU-based memory systems, with ARM, Thumb (Thumb-2 technology) and ThumbEE, DSP support, and 32-bit SIMD support.  */
  { "7-A.security", NULL, kArchExt_v7A | kCPUExt_Sec, kArchFPUExt_None }, /* ThumbEE required, VFPv3, optional security extensions.  */
  { "7-R", NULL, kArchExt_v7R, kArchFPUExt_None }, /* ARMv7 real-time profile with ARM, Thumb-2, DSP support, and 32-bit SIMD support (ThumbEE optional, VFPv3)  */
  { "7E-M", NULL, kArchExt_v7EM, kArchFPUExt_None }, /* ARMv7-M enhanced with DSP (saturating and 32-bit SIMD) instructions. Cortex-M4 */
  { "7-M", NULL, kArchExt_v7M, kArchFPUExt_None }, /* ARMv7 micro-controller profile with Thumb-2 only and hardware divide.  */
  
  /* Cores and CPU names: */
  /* v1 */
  { "ARM1", &oCPUArchList[eArch_1], 0, kArchFPUExt_None },

  /* v2 / v2A */
  { "ARM2", &oCPUArchList[eArch_2], 0, kArchFPUExt_None },
  { "ARM3", &oCPUArchList[eArch_2a], 0, kArchFPUExt_None },

  /* v3 */
  { "ARM6", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM60", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM600", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM610", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM620", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM7", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM7D", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM7DI", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM70", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM700", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM704", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM700I", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM710", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM710a", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM710C", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },
  { "ARM720", &oCPUArchList[eArch_3], 0, kArchFPUExt_None },

  /* v3M */
  { "ARM7M", &oCPUArchList[eArch_3M], 0, kArchFPUExt_None },
  { "ARM7DM", &oCPUArchList[eArch_3M], 0, kArchFPUExt_None },
  { "ARM7DMI", &oCPUArchList[eArch_3M], 0, kArchFPUExt_None },

  /* v4 */
  { "ARM8", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "ARM810", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "StrongARM", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "StrongARM1", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "SA-110", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "SA-1100", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "SA-1110", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "FA526", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  { "FA626", &oCPUArchList[eArch_4], 0, kArchFPUExt_None },
  
  /* v4T */
  { "ARM7TM", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM7TM-S", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM7TDM", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM7TDMI", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM7TDMI-S", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM710T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM720T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM740T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM9", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM9TDMI", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM910T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM920", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM920T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM922T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM925T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "ARM940T", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "EP9312", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },
  { "SC100", &oCPUArchList[eArch_4T], 0, kArchFPUExt_None },

  /* v4TxM */
  { "ARM7T", &oCPUArchList[eArch_4TxM], 0, kArchFPUExt_None },
  { "ARM7TDI", &oCPUArchList[eArch_4TxM], 0, kArchFPUExt_None },

  /* v5T */
  { "ARM10TDMI", &oCPUArchList[eArch_5T], 0, kArchFPUExt_VFPv1 },
  { "ARM1020T", &oCPUArchList[eArch_5T], 0, kArchFPUExt_VFPv1 },

  /* v5TE */
  { "88FR101", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR101.hw_divide", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR121", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR121.hw_divide", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR131", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR131.hw_divide", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR301", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR301.hw_divide", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR321", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR321.hw_divide", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR331", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "88FR331.hw_divide", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM9E", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM9E-S", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM946E-S", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM966E-S", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM968E-S", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM10E", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM1020E", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "ARM1022E", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "XScale", &oCPUArchList[eArch_5TE], kCPUExt_XScale, kArchFPUExt_VFPv2 },
  { "iWMMXt", &oCPUArchList[eArch_5TE], kCPUExt_XScale | kCPUExt_iWMMXt, kArchFPUExt_VFPv2 },
  { "iWMMXt2", &oCPUArchList[eArch_5TE], kCPUExt_XScale | kCPUExt_iWMMXt | kCPUExt_iWMMXt2, kArchFPUExt_VFPv2 },
  { "FA606TE", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "FA626TE", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "FMP626", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },
  { "FA726TE", &oCPUArchList[eArch_5TE], 0, kArchFPUExt_VFPv2 },

  /* v5TEJ */
  { "ARM7EJ-S", &oCPUArchList[eArch_5TEJ], 0, kArchFPUExt_None },
  { "ARM9EJ-S", &oCPUArchList[eArch_5TEJ], 0, kArchFPUExt_VFPv2 },
  { "ARM926EJ-S", &oCPUArchList[eArch_5TEJ], 0, kArchFPUExt_VFPv2 },
  { "ARM10EJ-S", &oCPUArchList[eArch_5TEJ], 0, kArchFPUExt_VFPv2 },
  { "ARM1026EJ-S", &oCPUArchList[eArch_5TEJ], 0, kArchFPUExt_VFPv2 },
  { "SC200", &oCPUArchList[eArch_5TEJ], 0, kArchFPUExt_VFPv2 },

  /* v6 */
  { "ARM1136J-S", &oCPUArchList[eArch_6], 0, kArchFPUExt_None }, /* revision 0 */
  { "ARM1136JF-S", &oCPUArchList[eArch_6], 0, kArchFPUExt_VFPv2 },

  /* v6K */
  { "ARM11MPCore", &oCPUArchList[eArch_6K], 0, kArchFPUExt_VFPv2 },
  { "ARM1136J-S-rev1", &oCPUArchList[eArch_6K], 0, kArchFPUExt_VFPv2 }, /* revision 1 */
  { "ARM1136JF-S-rev1", &oCPUArchList[eArch_6K], 0, kArchFPUExt_VFPv2 }, /* revision 1 */
  { "MPCore", &oCPUArchList[eArch_6K], 0, kArchFPUExt_VFPv2 },
  { "MPCore.no_vfp", &oCPUArchList[eArch_6K], 0, kArchFPUExt_None },
  
  /* v6Z */
  { "ARM1176JZ-S", &oCPUArchList[eArch_6Z], 0, kArchFPUExt_None },
  { "ARM1176JZF-S", &oCPUArchList[eArch_6Z], 0, kArchFPUExt_VFPv2 },

  /* v6T2 */
  { "88FR111", &oCPUArchList[eArch_6T2], 0, kArchFPUExt_VFPv2 },
  { "88FR111.no_hw_divide", &oCPUArchList[eArch_6T2], 0, kArchFPUExt_VFPv2 },
  { "ARM1156T2-S", &oCPUArchList[eArch_6T2], 0, kArchFPUExt_None },
  { "ARM1156T2F-S", &oCPUArchList[eArch_6T2], 0, kArchFPUExt_VFPv2 },

  /* v6-M */
  { "Cortex-M1.no_os_extension", &oCPUArchList[eArch_6M], 0, kArchFPUExt_None },
  { "SC000", &oCPUArchList[eArch_6M], 0, kArchFPUExt_None }, 

  /* v6S-M */
  { "Cortex-M0", &oCPUArchList[eArch_6SM], 0, kArchFPUExt_None },
  { "Cortex-M1", &oCPUArchList[eArch_6SM], 0, kArchFPUExt_None },
  { "Cortex-M1.os_extension", &oCPUArchList[eArch_6SM], 0, kArchFPUExt_None },

  /* V7-A */
  { "Cortex-A5", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_None },
  { "Cortex-A5.neon", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_VFPv4 | kFPUExt_NEONv1 | kFPUExt_NEON_FMA },
  { "Cortex-A5.vfp", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_VFPv4D16 },
  { "Cortex-A7", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_Div | kCPUExt_Virt | kCPUExt_ThumbEE, kArchFPUExt_VFPv4 },
  { "Cortex-A8", &oCPUArchList[eArch_7A], kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_VFPv3 | kFPUExt_NEONv1 },
  { "Cortex-A8.no_neon", &oCPUArchList[eArch_7A], kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_VFPv3 },
  { "Cortex-A9", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_VFPv3 | kFPUExt_FP16 | kFPUExt_NEONv1 },
  { "Cortex-A9.no_neon", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_VFPv3D16 | kFPUExt_FP16 },
  { "Cortex-A9.no_neon.no_vfp", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_ThumbEE, kArchFPUExt_None },
  { "Cortex-A15", &oCPUArchList[eArch_7A], kCPUExt_MP | kCPUExt_Sec | kCPUExt_Div | kCPUExt_Virt | kCPUExt_ThumbEE, kArchFPUExt_VFPv4 },
  { "QSP", &oCPUArchList[eArch_7A], kCPUExt_ThumbEE, kArchFPUExt_VFPv3 | kFPUExt_FP16 | kFPUExt_NEONv1 },
  { "QSP.no_neon", &oCPUArchList[eArch_7A], kCPUExt_ThumbEE, kArchFPUExt_VFPv3 | kFPUExt_FP16 },
  { "QSP.no_neon.no_vfp", &oCPUArchList[eArch_7A], kCPUExt_ThumbEE, kArchFPUExt_None },

  /* v7-R */
  { "Cortex-R4", &oCPUArchList[eArch_7R], 0, kArchFPUExt_None },
  { "Cortex-R4F", &oCPUArchList[eArch_7R], 0, kArchFPUExt_VFPv3D16 },
  { "Cortex-R5", &oCPUArchList[eArch_7R], kCPUExt_Div, kArchFPUExt_None },
  { "Cortex-R7", &oCPUArchList[eArch_7R], 0, kArchFPUExt_None },
  { "Cortex-R7F", &oCPUArchList[eArch_7R], 0, kArchFPUExt_VFPv3D16 | kFPUExt_FP16 },

  /* v7-M */
  { "Cortex-M3", &oCPUArchList[eArch_7M], 0, kArchFPUExt_None },
  { "Cortex-M3-rev0", &oCPUArchList[eArch_7M], 0, kArchFPUExt_None },
  { "SC300", &oCPUArchList[eArch_7M], 0, kArchFPUExt_None },

  /* v7-EM */
  { "Cortex-M4", &oCPUArchList[eArch_7EM], 0, kArchFPUExt_None },
  { "Cortex-M4.fp", &oCPUArchList[eArch_7EM], 0, kArchFPUExt_VFPv4SPD16 }
};

/* Optionally prefixed by "SoftFPA+" or "SoftVFP+".  */
static const FPU_ListEnty_t oFPUList[] =
{
  { "None", kArchFPUExt_None },
  { "FPE2", kFPUExt_FPAv1 },
  { "FPE3", kArchFPUExt_FPA }, /* LFM/SFM support.  */
  { "FPA10", kArchFPUExt_FPA }, /* for ARMv2 (ARM3) */
  { "FPA11", kArchFPUExt_FPA }, /* for ARMv3 (ARM700, ARM7500) */
  { "VFPv1", kArchFPUExt_VFPv1 },
  { "VFPv1-SP", kArchFPUExt_VFPv1xD },
  { "VFPv2", kArchFPUExt_VFPv2 }, /* VFPv2 architecture.  */
  { "VFPv3", kArchFPUExt_VFPv3 }, /* VFPv3 architecture. VFPv3 is backwards compatible with VFPv2 except that VFPv3 cannot trap floating-point exceptions.  */
  { "VFPv3_FP16", kArchFPUExt_VFPv3 | kFPUExt_FP16 }, /* VFPv3 architecture + half-precision extension.  */
  { "VFPv3_D16", kArchFPUExt_VFPv3D16 }, /* VFPv3-D16 architecture.  */
  { "VFPv3_D16_FP16", kArchFPUExt_VFPv3D16 | kFPUExt_FP16 }, /* VFPv3-D16 architecture + half-precision extension.  */
  { "VFPv3-SP", kArchFPUExt_VFPv3xD }, /* Single precision variant of the VFPv3 architecture.  */
  { "VFPv3-SP_FP16", kArchFPUExt_VFPv3xD | kFPUExt_FP16 }, /* Single precision variant of the VFPv3 architecture + half-precision extension.  */
  { "VFPv4", kArchFPUExt_VFPv4 }, /* FPv4 architecture.  */
  { "VFPv4_D16", kArchFPUExt_VFPv4D16 }, /* VFPv4-D16 architecture.  */
  { "FPv4-SP", kArchFPUExt_VFPv4SPD16 } /* Single precision variant of the FPv4 architecture.  */
};

/* Maps SOC devices to ARM cores.  */
static const Device_ListEntry_t oDeviceList[] =
{
  { "ARM250", "ARM3", "FPE2" },
  { "ARM7100", "ARM710a", "FPE2" },
  { "ARM7500", "ARM710C", "FPE2" },
  { "ARM7500FE", "ARM710C", "FPA11" },

  /* OMAP 1 */
  { "OMAP1510", "ARM925T", NULL },
  { "OMAP5910", "ARM925T", NULL },
  { "OMAP1610", "ARM926EJ-S", NULL },
  { "OMAP1611", "ARM926EJ-S", NULL },
  { "OMAP1612", "ARM926EJ-S", NULL },
  { "OMAP1621", "ARM926EJ-S", NULL },
  { "OMAP1710", "ARM926EJ-S", NULL },
  { "OMAP5912", "ARM926EJ-S", NULL },

  /* OMAP 2 */
  { "OMAP2420", "ARM1136JF-S", NULL },
  { "OMAP2430", "ARM1136JF-S", NULL },
  { "OMAP2431", "ARM1136JF-S", NULL },

  /* OMAP 3 */
  { "OMAP34xx", "Cortex-A8", NULL },
  { "OMAP35xx", "Cortex-A8", NULL },
  { "OMAP36xx", "Cortex-A8", NULL },

  /* OMAP 4 */
  { "OMAP4xxx", "Cortex-A9", "VFPv3_FP16" },

  /* OMAP 5 */
  { "OMAP5xxx", "Cortex-A15", NULL },

  /* OMAP multimedia.  */
  { "OMAP-DM270", "ARM7TDMI", NULL },
  { "OMAP-DM299", "ARM7TDMI", NULL },
  { "OMAP-DM500", "ARM7TDMI", NULL },
  { "OMAP310", "ARM925T", NULL },
  { "OMAP331", "ARM926EJ-S", NULL },
  { "OMAP-DM510", "ARM926EJ-S", NULL },
  { "OMAP-DM515", "ARM926EJ-S", NULL },
  { "OMAP-DM525", "ARM926EJ-S", NULL },

  /* OMAP modem.  */
  { "OMAP710", "ARM925T", NULL },
  { "OMAP730", "ARM926EJ-S", NULL },
  { "OMAP733", "ARM926EJ-S", NULL },
  { "OMAP750", "ARM926EJ-S", NULL },
  { "OMAP850", "ARM926EJ-S", NULL },
  { "OMAPV1030", "ARM926EJ-S", NULL },
  { "OMAPV1035", "ARM926EJ-S", NULL },

  /* OMAP L-1x.  */
  { "OMAP-L137", "ARM926EJ-S", NULL },
  { "OMAP-L138", "ARM926EJ-S", NULL },

  /* XScale : Application Processors.  */
  { "PXA215", "XScale", NULL },
  { "PXA255", "XScale", NULL },
  { "PXA260", "XScale", NULL },
  { "PXA270", "iWMMXt", NULL },
  { "PXA800F", "XScale", NULL },

  { "IQ80200", "XScale", NULL },
  { "IQ80321", "XScale", NULL },
  { "IQ80325", "XScale", NULL },
  { "IQ80326", "XScale", NULL },
  { "IQ80327", "XScale", NULL },

  /* XScale : Network Processors.  */
  { "IXP42x", "XScale", NULL },
  { "IXP46x", "XScale", NULL },
  { "IXP23xx", "XScale", NULL },
  { "IXP2400", "XScale", NULL },

  { "iWMMXt", "iWMMXt", NULL },
  { "iWMMXt2", "iWMMXt2", NULL },

  /* Freescale semiconductor.  */
  { "i.MX21", "ARM926EJ-S", NULL },
  { "i.MX31", "ARM1136JF-S", NULL },
  { "i.MX51", "Cortex-A8", NULL }
};

/**
 * Sets the CPU/architecture to target for.
 * \param cpu CPU/architecture to target for.  Some CPU selections will imply a
 * FPU selection.  When "list" it will output the list of supported CPU and
 * architectures.
 * \param fpu FPU to target for.
 * \return true when failure (or after "list"), false otherwise.
 * "If no fpu and no cpu are specified, -fpu softvfp is used." ???
 */
bool
Target_SetCPU_FPU_Device (const char *cpu, const char *fpu, const char *device)
{
  bool rtrn = false;
  if (cpu && !strcmp (cpu, "list"))
    {
      fprintf (stderr, "Supported CPUs : ");
      for (size_t i = 0; i != sizeof (oCPUArchList)/sizeof (oCPUArchList[0]); ++i)
	{
	  fputs (oCPUArchList[i].cpu_arch, stderr);
	  if (i != sizeof (oCPUArchList)/sizeof (oCPUArchList[0]) - 1)
	    fputs (", ", stderr);
	}
      fputc ('\n', stderr);
      rtrn = true;
    }
  if (fpu && !strcmp (fpu, "list"))
    {
      fprintf (stderr, "Supported FPU values : ");
      for (unsigned s = 0; s != 3; ++s)
	{
	  const char *prefix = (s == 0) ? "" : (s == 1) ? "SoftFPA" : "SoftVFP";
	  for (size_t i = 0; i != sizeof (oFPUList)/sizeof (oFPUList[0]); ++i)
	    {
	      if (i == 0)
		fputs (s == 0 ? oFPUList[i].fpu : prefix, stderr);
	      else
		{
		  fputs (prefix, stderr);
		  if (s)
		    fputc ('+', stderr);
		  fputs (oFPUList[i].fpu, stderr);
		}
	      if (s != 2 || i != sizeof (oFPUList)/sizeof (oFPUList[0]) - 1)
		fputs (", ", stderr);
	    }
	}
      fputc ('\n', stderr);
      rtrn = true;
    }
  if (device && !strcmp (device, "list"))
    {
      fprintf (stderr, "Supported Device values : ");
      for (size_t i = 0; i != sizeof (oDeviceList)/sizeof (oDeviceList[0]); ++i)
	{
	  fputs (oDeviceList[i].device, stderr);
	  if (i != sizeof (oDeviceList)/sizeof (oDeviceList[0]) - 1)
	    fputs (", ", stderr);
	}
      fputc ('\n', stderr);
      rtrn = true;
    }
  if (rtrn)
    return true;

  if (device != NULL)
    {
      const Device_ListEntry_t *devP = NULL;
      for (size_t i = 0; i != sizeof (oDeviceList)/sizeof (oDeviceList[0]); ++i)
	{
	  if (!strcasecmp (device, oDeviceList[i].device))
	    {
	      devP = &oDeviceList[i];
	      break;
	    }
	}
      if (devP == NULL)
	{
	  fprintf (stderr, "Unknown Device %s\n", device);
	  return true;
	}
      oTarget_Device = devP;
      if (cpu != NULL)
	fprintf (stderr, "Warning: Option device %s will overrule option cpu %s\n", device, cpu);
      cpu = devP->core;
      if (fpu != NULL)
	fprintf (stderr, "Warning: Option device %s will overrule option fpu %s\n", device, fpu);
      fpu = devP->fpu;
    }
  
  /* When CPU option is not specified, ObjAsm defaults to "3" while armasm
     defaults to "ARM7TDMI".  */
  if (cpu == NULL)
    cpu = "3";
  unsigned cpu_features;
  const CPUArch_ListEntry_t *selCPUArchP = NULL;
  for (size_t i = 0; i != sizeof (oCPUArchList)/sizeof (oCPUArchList[0]); ++i)
    {
      if (!strcasecmp (cpu, oCPUArchList[i].cpu_arch))
	{
	  selCPUArchP = &oCPUArchList[i];

	  /* Determine CPU features.  */
	  cpu_features = selCPUArchP->cpu_features;
	  if (selCPUArchP->archP != NULL)
	    cpu_features |= selCPUArchP->archP->cpu_features;
	  break;
	}
    }
  if (selCPUArchP == NULL)
    {
      fprintf (stderr, "Unknown CPU/Architecture %s\n", cpu);
      return true;
    }
  oTarget_CPU = selCPUArchP;
  oTarget_CPUFeatures = cpu_features;

  const FPU_ListEnty_t *selFPUP = NULL;
  if (fpu == NULL)
    {
      /* When FPU option is not specified, ObjAsm defaults to "FPE2" for ARM cores,
	 "None" for Thumb-only cores, while armasm defaults to "SoftVFP".
         For VFP capable CPUs, AsAsm defaults to the real hardware capabilities
         of the chosen CPU.  */
      if (oTarget_CPU->fpu_features == kArchFPUExt_None)
	fpu = (Target_GetCPUFeatures () & kCPUExt_v1) ? "FPE2" : "None";
      else
	{
	  for (selFPUP = &oFPUList[0]; selFPUP != &oFPUList[sizeof (oFPUList)/sizeof (oFPUList[0])]; ++selFPUP)
	    {
	      if (selFPUP->features == (oTarget_CPU->fpu_features & kFPUExt_MaskVFPFPA))
		break;
	    }
	  assert (selFPUP != &oFPUList[sizeof (oFPUList)/sizeof (oFPUList[0])]);
	  oTarget_FPUFeatures = oTarget_CPU->fpu_features;
	}
    }
  if (selFPUP == NULL)
    {
      /* Strip off any SoftFPA(+) or SoftVFP(+) prefix.  */
      const char *fpu_parse = fpu;
      unsigned features;
      if (!strncasecmp (fpu_parse, "SoftFPA", sizeof ("SoftFPA")-1))
	{
	  features = kFPUExt_SoftFPA;
	  fpu_parse += sizeof ("SoftFPA")-1;
	}
      else if (!strncasecmp (fpu_parse, "SoftVFP", sizeof ("SoftVFP")-1))
	{
	  features = kFPUExt_SoftVFP;
	  fpu_parse += sizeof ("SoftVFP")-1;
	}
      else
	features = kArchFPUExt_None;
      size_t i;
      if (features != kArchFPUExt_None)
	{
	  if (fpu_parse[0] == '\0')
	    selFPUP = &oFPUList[0];
	  else
	    {
	      i = 1; /* Skip "None".  */
	      if (fpu_parse[0] == '+')
		++fpu_parse;
	    }
	}
      else
	i = 0;
      if (selFPUP == NULL)
	{
	  for (/* */; i != sizeof (oFPUList)/sizeof (oFPUList[0]); ++i)
	    {
	      if (!strcasecmp (fpu_parse, oFPUList[i].fpu))
		{
		  selFPUP = &oFPUList[i];
		  break;
		}
	    }
	  if (selFPUP == NULL)
	    {
	      fprintf (stderr, "Unknown FPU %s\n", fpu);
	      return true;
	    }
	}
      /* The --fpu option will not (re)set any NEON features.  */
      features |= (oTarget_CPU->fpu_features & kFPUExt_MaskNEON) | selFPUP->features;
      if (features & kFPUExt_SoftFPA)
	features &= ~kFPUExt_NoEndianMismatch;
      else if (features & kFPUExt_SoftVFP)
	features |= kFPUExt_NoEndianMismatch;
      oTarget_FPUFeatures = features;
    }
  oTarget_FPU = selFPUP;

  return false;
}


/**
 * \return If an architecture was specified in the command line -cpu option,
 * "Generic ARM" is returned.
 */
const char *
Target_GetCPU (void)
{
  return oTarget_CPU->archP == NULL ? "Generic ARM" : oTarget_CPU->cpu_arch;
}


const char *
Target_GetArch (bool qualifiers)
{
  /* Hack to have as ARCH "7-A" for "7-A.security".  */
  if (!qualifiers && oTarget_CPU == &oCPUArchList[eArch_7A_security])
    return oCPUArchList[eArch_7A].cpu_arch;

  return oTarget_CPU->archP == NULL ? oTarget_CPU->cpu_arch : oTarget_CPU->archP->cpu_arch;
}


const char *
Target_GetFPU (void)
{
  return oTarget_FPU->fpu;
}


unsigned
Target_GetARMISAVersion (void)
{
  unsigned version;
  unsigned features = Target_GetCPUFeatures ();
  if (!(features & kCPUExt_v1))
    version = 0; /* No ARM ISA.  */
  else if ((features & (kCPUExt_v7A | kCPUExt_v7R)) != 0)
    version = 7; /* ARMv7-A, ARMv7-R.  */
  else if ((features & kCPUExt_v6) != 0)
    version = 6; /* ARMv6*.  */
  else if ((features & kCPUExt_v5) != 0)
    version = 5; /* ARMv5*.  */
  else if ((features & kCPUExt_v4) != 0)
    version = 4; /* ARMv4*.  */
  else if ((features & kCPUExt_v3) != 0)
    version = 3; /* ARMv3*.  */
  else if ((features & kCPUExt_v2) != 0)
    version = 2; /* ARMv2*.  */
  else
    version = 1; /* ARMv1.  */
  return version;
}


unsigned
Target_GetThumbISAVersion (void)
{
  unsigned version;
  unsigned features = Target_GetCPUFeatures ();
  if ((features & kCPUExt_v6T2) != 0)
    version = 4;
  else if ((features & kCPUExt_v6) != 0)
    version = 3;
  else if ((features & kCPUExt_v5T) != 0)
    version = 2;
  else if ((features & kCPUExt_v4T) != 0)
    version = 1;
  else
    version = 0;
  return version;
}


/**
 * Check if given set of features is fully supported by the currently selected
 * CPU/Architecture.
 * \param features One or more kCPUExt_* bits.
 * \param warn When true, a warning will be generated if not all the features
 * are supported.
 * \return true when all features are supported, false otherwise.
 */
bool
Target_CheckCPUFeature (unsigned features, bool warn)
{
  bool missingFeature = (Target_GetCPUFeatures () & features) != features;
  if (warn && missingFeature)
    {
      if (oTarget_Device != NULL)
	error (ErrorWarning, "Instruction is not supported on selected device %s (core %s, architecture %s)",
	       oTarget_Device->device, oTarget_CPU->cpu_arch, Target_GetArch (true));
      else if (oTarget_CPU->archP == NULL)
	error (ErrorWarning, "Instruction is not supported on selected architecture %s",
	       oTarget_CPU->cpu_arch);
      else
	error (ErrorWarning, "Instruction is not supported on selected core %s (architecture %s)",
	       oTarget_CPU->cpu_arch, oTarget_CPU->archP->cpu_arch);
    }
  return !missingFeature;
}


/**
 * Check if given set of features is fully supported by the currently selected
 * FPU.
 * \param features One or more kFPUExt_* bits.
 * \param warn When true, a warning will be generated if not all the features
 * are supported.
 * \return true when all features are supported, false otherwise.
 */
bool
Target_CheckFPUFeature (unsigned features, bool warn)
{
  bool missingFeature = (oTarget_FPUFeatures & features) != features;
  if (warn && missingFeature)
    error (ErrorWarning, "Instruction is not supported on selected FPU %s",
	   oTarget_FPU->fpu);
  return !missingFeature;
}


unsigned
Target_GetCPUFeatures (void)
{
  return oTarget_CPUFeatures;
}


unsigned
Target_GetFPUFeatures (void)
{
  return oTarget_FPUFeatures;
}
