/*
 * AS an assembler for ARM
 * Copyright (c) 2010-2013 GCCSDK Developers
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
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

/* ARM CPU extensions: */
#define kCPUExt_v1        0x0000000001ULL /* ARM ISA.  */
#define kCPUExt_v2        0x0000000002ULL /* MUL/MLA.  */
#define kCPUExt_v2a	  0x0000000004ULL /* SWP[B].  */
#define kCPUExt_v3	  0x0000000008ULL /* MSR/MRS.  */
#define kCPUExt_v3M	  0x0000000010ULL /* Long multiply instructions.  */

#define kCPUExt_v4	  0x0000000020ULL /* Half word.  */
#define kCPUExt_v4T	  0x0000000040ULL /* Thumb.  */
#define kCPUExt_v5	  0x0000000080ULL /* CLZ, BKPT.  */
#define kCPUExt_v5T	  0x0000000100ULL /* Improved ARM/Thumb interworking, LDC2/STC2  */
#define kCPUExt_v5ExP	  0x0000000200ULL /* DSP (mul/acc on 16bit, saturated arithmetic).  */
#define kCPUExt_v5E	  0x0000000400ULL /* LDRD/STRD, MCRR/MRRC, PLD.  */
#define kCPUExt_v5J	  0x0000000800ULL /* BXJ (Jazelle).  */
#define kCPUExt_v6        0x0000001000ULL /* ARM: CPS/SRS/RFE/REV/REV16/REVSH/SETEND/LDREX/STREX/SXTB/SXTH/UXTB/UXTH, SIMD, multiply. Thumb: CPS/CPY/REV/REV16/REVSH/SETEND/SXTB/SXTH/UXTB/UXTH. */
#define kCPUExt_v6K       0x0000002000ULL /* ARMv6K (multiprocessing support), memory model features.  */

#define kCPUExt_v6T2	  0x0000004000ULL /* Thumb-2.  */
#define kCPUExt_v7A	  0x0000008000ULL /* ARMv7-A.  */
#define kCPUExt_v7R	  0x0000010000ULL /* ARMv7-R.  */
#define kCPUExt_v7M	  0x0000020000ULL /* ARMv7-M.  */
#define kCPUExt_v6M	  0x0000040000ULL /* ARM V6M.  */
#define kCPUExt_BARRIER	  0x0000080000ULL /* DSB/DMB/ISB.  */
#define kCPUExt_THUMB_MSR 0x0000100000ULL /* Thumb MSR/MRS.  */
#define kCPUExt_v6_DSP    0x0000200000ULL /* ARM v6 (DSP-related), not in v7-M.  */

#define kCPUExt_Div	  0x0000400000ULL /* SDIV/UDIV.  */
#define kCPUExt_ADIV	  0x0000800000ULL /* Integer divide extensions in ARM state.  */
#define kCPUExt_MP        0x0001000000ULL /* Multiprocessing Extensions.  */
#define kCPUExt_Sec	  0x0002000000ULL /* Security extensions (SMC/SMI).  */
#define kCPUExt_OS	  0x0004000000ULL /* OS Extensions.  */
#define kCPUExt_Virt	  0x0008000000ULL /* Virtualization extensions.  */
#define kCPUExt_ThumbEE	  0x0010000000ULL /* ThumbEE, optionally from ARMv7 onwards.  */

#define kCPUExt_v8	  0x0020000000ULL /* ARMv8 (AArch32 - A32, T32 and T32EE).  */
#define kCPUExt_v8A	  0x0040000000ULL /* ARMv8-A */

#define kCPUExt_XScale	  0x2000000000ULL /* XScale extensions.  */
#define kCPUExt_iWMMXt	  0x4000000000ULL /* Intel Wireless MMX technology coprocessor.  */
#define kCPUExt_iWMMXt2   0x8000000000ULL /* Intel Wireless MMX technology coprocessor v2.  */

/* kCPUExt_Div : ARM DDI 0406C A4.4.8
    - ARMv7-R profile => SDIV/UDIV implemented in Thumb.
    - AMRv7-R profile => SDIV/UDIV in ARM optional
    - Virtualization Extensions => SDIV/UDIV implemented in Thumb and ARM.
    - ARMv7-A profile : implementation dependant when no Virtualization Extensions.  */

/* ARM architecture defines: */
#define kArchExt_v1       (kCPUExt_v1)

#define kArchExt_v2       (kArchExt_v1 | kCPUExt_v2) /* v1 + MUL/MLA.  */
#define kArchExt_v2a      (kArchExt_v2 | kCPUExt_v2a) /* v2 + SWP[B].  */

#define kArchExt_v3       (kArchExt_v2a | kCPUExt_v3) /* v2s + MSR/MRS.  */
#define kArchExt_v3M      (kArchExt_v3 | kCPUExt_v3M) /* v3 + long multiply.  */

#define kArchExt_v4xM     (kArchExt_v3 | kCPUExt_v4) /* v3 + half word.  */
#define kArchExt_v4       (kArchExt_v3M | kCPUExt_v4) /* v3M + half word.  */
#define kArchExt_v4TxM    (kArchExt_v4xM | kCPUExt_v4T) /* v4xM + Thumb.  */
#define kArchExt_v4T      (kArchExt_v4 | kCPUExt_v4T) /* v4 + Thumb.  */

#define kArchExt_v5xM     (kArchExt_v4xM | kCPUExt_v5) /* v4xM + CLZ/BKPT.  */
#define kArchExt_v5       (kArchExt_v4 | kCPUExt_v5) /* v4 + CLZ/BKPT.  */
#define kArchExt_v5TxM    (kArchExt_v5xM | kCPUExt_v4T | kCPUExt_v5T) /* v5xM + Thumb + interworking.  */
#define kArchExt_v5T      (kArchExt_v5 | kCPUExt_v4T | kCPUExt_v5T) /* v5 + Thumb + interworking.  */
#define kArchExt_v5TExP   (kArchExt_v5T | kCPUExt_v5ExP) /* v5T + DSP.  */
#define kArchExt_v5TE     (kArchExt_v5TExP | kCPUExt_v5E) /* v5TExP + LDRD/STRD/MCRR/MRRC/PLD.  */
#define kArchExt_v5TEJ    (kArchExt_v5TE | kCPUExt_v5J) /* v5TE + Jazelle.  */

#define kArchExt_v6       (kArchExt_v5TEJ | kCPUExt_v6) /* v5TEJ + base ARMv6.  */
#define kArchExt_v6K      (kArchExt_v6 | kCPUExt_v6K) /* v6 + base ARMv6K.  */
#define kArchExt_v6Z      (kArchExt_v6 | kCPUExt_Sec) /* v6 + security extensions.  */
#define kArchExt_v6KZ     (kArchExt_v6Z | kCPUExt_Sec) /* v6Z + security extensions.  */
#define kArchExt_v6T2     (kArchExt_v6 | kCPUExt_v6T2)
#define kArchExt_v6M      (kCPUExt_v4T | kCPUExt_v5 | kCPUExt_v5T | kCPUExt_v5E \
  | kCPUExt_v6 | kCPUExt_v6K | kCPUExt_v6M | kCPUExt_BARRIER | kCPUExt_THUMB_MSR)
#define kArchExt_v6SM     (kArchExt_v6M | kCPUExt_OS)
#define kArchExt_v7       (kCPUExt_v4T | kCPUExt_v5 | kCPUExt_v5T | kCPUExt_v5E \
  | kCPUExt_v6 | kCPUExt_v6K | kCPUExt_v6T2 | kCPUExt_BARRIER | kCPUExt_THUMB_MSR)
#define kArchExt_v7A      (kArchExt_v6T2 | kCPUExt_v6K | kArchExt_v7 | kCPUExt_BARRIER | kCPUExt_v7A)
#define kArchExt_v7R      (kArchExt_v6T2 | kCPUExt_v6K | kArchExt_v7 | kCPUExt_BARRIER | kCPUExt_Div | kCPUExt_v7R)
#define kArchExt_v7M      (kCPUExt_v4T | kCPUExt_v5 | kCPUExt_v5T | kCPUExt_v5E \
  | kCPUExt_v6 | kCPUExt_v6K \
  | kCPUExt_v6T2 | kCPUExt_Div | kCPUExt_v7M \
  | kCPUExt_v6M | kCPUExt_BARRIER | kCPUExt_THUMB_MSR)
#define kArchExt_v7EM     (kArchExt_v7M | kCPUExt_v5ExP | kCPUExt_v6_DSP)
#define kArchExt_v8       ((kArchExt_v7A & ~kCPUExt_v7A) | kCPUExt_MP | kCPUExt_Sec \
  | kCPUExt_Div | kCPUExt_ADIV | kCPUExt_Virt | kCPUExt_v8) 
#define kArchExt_v8A	  (kArchExt_v8 | kCPUExt_v8A)

/* ARM FPU extension: */
#define kFPUExt_NoEndianMismatch 0x00000001U /* Double-precision words are stored according to target endian.  */
#define kFPUExt_FPAv1            0x00000002U /* Base FPA.  */
#define kFPUExt_FPAv2            0x00000004U /* LFM/SFM.  */

#define kFPUExt_VFPv1xD          0x00000008U /* VFP v1 single precision.  */
#define kFPUExt_VFPv1            0x00000010U /* VFP v1 single + double precision.  */
#define kFPUExt_VFPv2            0x00000020U /* VFP v2.  */
#define kFPUExt_VFPv3xD          0x00000040U /* VFP v3 single precision.  */
#define kFPUExt_VFPv3            0x00000080U /* VFP v3 double precision.  */
#define kFPUExt_D16D32           0x00000100U /* VFP regs D16-D31.  */
#define kFPUExt_FP16             0x00000200U /* Half precision (always shared by VFPv3, VFPv4 and NEON).  */
#define kFPUExt_VFP_FMA          0x00000400U /* "Fused Multiply Add" VFP.  */
#define kFPUExt_VFP_ARMv8	 0x00000800U /* ARMv8 VFP.  */

#define kFPUExt_MaskVFPFPA	 0x00000FFFU /* All FPA + VFP related bits.  */

#define kFPUExt_NEONv1           0x00001000U /* NEON v1.  */
#define kFPUExt_NEON_FMA         0x00002000U /* "Fused Multiply Add" NEON.  */
#define kFPUExt_NEON_ARMv8	 0x00004000U /* ARMv8 NEON.  */

#define kFPUExt_MaskNEON	 0x00007000U /* All NEON related bits (we don't include kFPUExt_FP16).  */

#define kFPUExt_Crypto_ARMv8	 0x00010000U /* ARMv8 Crypto.  */

/* VFPv4(D16) = VFPv3(D16) + FP16 + VFP_FMA */
/* NEON v2 = NEON v1 + FP16 + NEON_FMA */
/* FIXME: Cirrus Maverick FPU.  */
#define kFPUExt_SoftVFP          0x40000000U
#define kFPUExt_SoftFPA          0x80000000U

#if 0
VFPv1 was implemented in the VFP10 revision 0 silicon (as provided by the
ARM10200).

VFPv2, implemented in:

 - VFP10 revision 1, as provided by the ARM10200E processor
 - VFP9-S, available as option for the ARM926E, ARM946E and ARM966E processors
 - VFP11, as provided in the ARM1136JF-S, ARM1176JZF-S and ARM11 MPCore processors.

  Is an optional extension to:
  - the ARM instruction set in the ARMv5TE, ARMv5TEJ, ARMv6, and ARMv6K architectures
  - the ARM and Thumb instruction sets in the ARMv6T2 architecture.

VFPv3, optional extension to ARMv7-A and ARMv7-R profile.
VFPv3 is backwards compatible with VFPv2, except that it cannot trap
floating point exceptions. It requires no software support code. VFPv3 has 32
double-precision registers.

VFPv3-D16, an implementation of VFPv3 that provides 16 double-precision
registers. It is implemented on ARM architecture v7 processors that support
VFP without NEON.

VFPv3U, an implementation of VFPv3 that can trap floating-point exceptions. It
requires software support code.

VFPv3_fp16, VFPv3 with half-precision extensions (note also VFPv3U + fp16 is possible).
These extensions provide
conversion functions between half-precision floating-point numbers and
single-precision floating-point numbers, in both directions. They can be
implemented with any Advanced SIMD and VFP implementation that supports
single-precision floating-point numbers.

VFPv4, optional extension for ARMv7-A and ARMv7-R, for example, the
Cortex-A7 processor. VFPv4 has 32 double-precision registers. VFPv4 adds both
half-precision extensions and fused multiply-add instructions to the features
of VFPv3.

VFPv4-D16, an implementation of VFPv4 that provides 16 double-precision
registers. It is implemented on ARM architecture v7 processors that support
VFP without NEON.

VFPv4U, an implementation of VFPv4 that can trap floating-point exceptions. It
requires software support code.

I.e. VFPv4-D32, or VFPv4U-D32, VFPv4-D16, or VFPv4U-D16.


If an implementation includes both the Floating-point and Advanced SIMD Extensions:
• It must implement the corresponding versions of the extensions:
— if the implementation includes VFPv3 it must include Advanced SIMDv1
— if the implementation includes VFPv3 with the Half-precision Extension it
   must include Advanced SIMDv1 with the half-precision extensions
— if the implementation includes VFPv4 it must include Advanced SIMDv2.
• The two extensions use the same register bank. This means VFP must be
implemented as VFPv3-D32, or as VFPv4-D32.
• Some instructions apply to both extensions.

#endif

#define kArchFPUExt_None         0
#define kArchFPUExt_FPA          (kFPUExt_FPAv1 | kFPUExt_FPAv2)
#define kArchFPUExt_VFPv1xD      (kFPUExt_NoEndianMismatch | kFPUExt_VFPv1xD)
#define kArchFPUExt_VFPv1        (kArchFPUExt_VFPv1xD | kFPUExt_VFPv1)
#define kArchFPUExt_VFPv2        (kArchFPUExt_VFPv1 | kFPUExt_VFPv2)
#define kArchFPUExt_VFPv3D16     (kArchFPUExt_VFPv2 | kFPUExt_VFPv3xD | kFPUExt_VFPv3)
#define kArchFPUExt_VFPv3        (kArchFPUExt_VFPv3D16 | kFPUExt_D16D32)
#define kArchFPUExt_VFPv3xD      (kFPUExt_NoEndianMismatch | kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)
#define kArchFPUExt_VFPv4D16     (kArchFPUExt_VFPv3D16 | kFPUExt_FP16 | kFPUExt_VFP_FMA) 
#define kArchFPUExt_VFPv4        (kArchFPUExt_VFPv3 | kFPUExt_FP16 | kFPUExt_VFP_FMA)
#define kArchFPUExt_VFPv4SPD16   (kArchFPUExt_VFPv3xD | kFPUExt_FP16 | kFPUExt_VFP_FMA)
#define kArchFPUExt_VFP_ARMv8    (kArchFPUExt_VFPv4 | kFPUExt_VFP_ARMv8)
#define kArchFPUExt_NEON_ARMv8   (kFPUExt_NEONv1 | kFPUExt_NEON_FMA | kFPUExt_NEON_ARMv8)
#define kArchFPUExt_Crypto_ARMv8 (kFPUExt_Crypto_ARMv8)

/* Permissible NEON & VFP combinations (ARMARM A2-4):
      NEON                 VFP
   1) None                 None
   2) Integer only         None
   3) Integer, single      Single
   4) Integer, single      Single, double
   5) None                 Single
   6) None                 Single, double
 */

bool Target_SetCPU_FPU_Device (const char *cpu, const char *fpu, const char *device);

const char *Target_GetCPU (void);
const char *Target_GetArch (bool qualifiers);
const char *Target_GetFPU (void);

unsigned Target_GetARMISAVersion (void);
unsigned Target_GetThumbISAVersion (void);

bool Target_CheckCPUFeature (uint64_t features, bool warn);
bool Target_CheckCPUFeatureDetail (uint64_t features, const char *what);
bool Target_CheckFPUFeature (unsigned features, bool warn);

uint64_t Target_GetCPUFeatures (void);
unsigned Target_GetFPUFeatures (void);

#endif
