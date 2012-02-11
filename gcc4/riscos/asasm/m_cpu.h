/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2012 GCCSDK Developers
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
 * m_cpu.h
 */

#ifndef m_cpu_header_included
#define m_cpu_header_included

#include <stdbool.h>

#include "decode.h"

#define M_MNEM	0x01e00000

#define M_ADC	0x00a00000
#define M_ADD	0x00800000
#define M_AND	0x00000000
#define M_BIC	0x01c00000
#define M_EOR	0x00200000
#define M_MOV	0x01a00000
#define M_MVN	0x01e00000
#define M_ORR	0x01800000
#define M_RSB	0x00600000
#define M_RSC	0x00e00000
#define M_SBC	0x00c00000
#define M_SUB	0x00400000
#define M_CMN	0x01600000
#define M_CMP	0x01400000
#define M_TEQ	0x01200000
#define M_TST	0x01000000

#define M_MLA	0x00200090
#define M_MLS	0x00600090
#define M_MUL	0x00000090

#define M_SMLABB 0x01000080
#define M_SMLATB 0x010000a0
#define M_SMLABT 0x010000c0
#define M_SMLATT 0x010000e0
#define M_SMLALBB 0x01400080
#define M_SMLALTB 0x014000a0
#define M_SMLALBT 0x014000c0
#define M_SMLALTT 0x014000e0
#define M_SMLAWB 0x01200080
#define M_SMLAWT 0x012000c0

#define M_SMULBB 0x01600080
#define M_SMULTB 0x016000a0
#define M_SMULBT 0x016000c0
#define M_SMULTT 0x016000e0
#define M_SMULWB 0x012000a0
#define M_SMULWT 0x012000e0

#define M_UMULL  0x00800090
#define M_UMLAL  0x00a00090
#define M_SMULL  0x00c00090
#define M_SMLAL  0x00e00090

#define M_CLZ	0x016F0F10

#define M_QADD   0x01000050
#define M_QDADD  0x01400050
#define M_QDSUB  0x01600050
#define M_QSUB   0x01200050

Rslt_e m_adc (bool doLowerCase);
Rslt_e m_add (bool doLowerCase);
Rslt_e m_and (bool doLowerCase);
Rslt_e m_bic (bool doLowerCase);
Rslt_e m_clz (bool doLowerCase);
Rslt_e m_cmn (bool doLowerCase);
Rslt_e m_cmp (bool doLowerCase);
Rslt_e m_eor (bool doLowerCase);
Rslt_e m_mla (bool doLowerCase);
Rslt_e m_mls (bool doLowerCase);
Rslt_e m_mov (bool doLowerCase);
Rslt_e m_mul (bool doLowerCase);
Rslt_e m_mvn (bool doLowerCase);
Rslt_e m_nop (bool doLowerCase);
Rslt_e m_orr (bool doLowerCase);
Rslt_e m_qadd (bool doLowerCase);
Rslt_e m_qdadd (bool doLowerCase);
Rslt_e m_qdsub (bool doLowerCase);
Rslt_e m_qsub (bool doLowerCase);
Rslt_e m_rsb (bool doLowerCase);
Rslt_e m_rsc (bool doLowerCase);
Rslt_e m_sbc (bool doLowerCase);
Rslt_e m_smull (bool doLowerCase);
Rslt_e m_smulbb (bool doLowerCase);
Rslt_e m_smulbt (bool doLowerCase);
Rslt_e m_smultb (bool doLowerCase);
Rslt_e m_smultt (bool doLowerCase);
Rslt_e m_smulwb (bool doLowerCase);
Rslt_e m_smulwt (bool doLowerCase);
Rslt_e m_smlal (bool doLowerCase);
Rslt_e m_smlalbb (bool doLowerCase);
Rslt_e m_smlalbt (bool doLowerCase);
Rslt_e m_smlaltb (bool doLowerCase);
Rslt_e m_smlaltt (bool doLowerCase);
Rslt_e m_smlabb (bool doLowerCase);
Rslt_e m_smlabt (bool doLowerCase);
Rslt_e m_smlatb (bool doLowerCase);
Rslt_e m_smlatt (bool doLowerCase);
Rslt_e m_smlawb (bool doLowerCase);
Rslt_e m_smlawt (bool doLowerCase);
Rslt_e m_sub (bool doLowerCase);
Rslt_e m_teq (bool doLowerCase);
Rslt_e m_tst (bool doLowerCase);
Rslt_e m_umull (bool doLowerCase);
Rslt_e m_umlal (bool doLowerCase);

Rslt_e m_asr (bool doLowerCase);
Rslt_e m_lsl (bool doLowerCase);
Rslt_e m_lsr (bool doLowerCase);
Rslt_e m_ror (bool doLowerCase);
Rslt_e m_rrx (bool doLowerCase);

#endif
