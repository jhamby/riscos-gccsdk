/*
 * AS an assembler for ARM
 * Copyright © 1992 Niklas Röjemo
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

#define M_MNEM 0x01e00000

#define M_ADC    0x00a00000
#define M_ADD    0x00800000
#define M_AND    0x00000000
#define M_BIC    0x01c00000
#define M_EOR    0x00200000
#define M_MOV    0x01a00000
#define M_MVN    0x01e00000
#define M_ORR    0x01800000
#define M_RSB    0x00600000
#define M_RSC    0x00e00000
#define M_SBC    0x00c00000
#define M_SUB    0x00400000
#define M_CMN    0x01600000
#define M_CMP    0x01400000
#define M_TEQ    0x01200000
#define M_TST    0x01000000
#define M_MLA    0x00200090
#define M_MUL    0x00000090

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

#endif
