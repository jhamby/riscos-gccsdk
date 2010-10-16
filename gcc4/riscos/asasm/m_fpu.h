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
 * m_fpu.h
 */

#ifndef m_fpu_header_included
#define m_fpu_header_included

#include "global.h"

ARMWord fpuImm (ARMFloat d) __attribute__ ((__const__));

#define M_FMNEM 0x0ef08110

#define M_ADF 0x0e000100
#define M_DVF 0x0e400100
#define M_FDV 0x0ea00100
#define M_FML 0x0e900100
#define M_FRD 0x0eb00100
#define M_MUF 0x0e100100
#define M_POL 0x0ec00100
#define M_POW 0x0e600100
#define M_RDF 0x0e500100
#define M_RMF 0x0e800100
#define M_RPW 0x0e700100
#define M_RSF 0x0e300100
#define M_SUF 0x0e200100
#define M_ABS 0x0e208100
#define M_ACS 0x0ec08100
#define M_ASN 0x0eb08100
#define M_ATN 0x0ed08100
#define M_COS 0x0e908100
#define M_EXP 0x0e708100
#define M_LGN 0x0e608100
#define M_LOG 0x0e508100
#define M_MNF 0x0e108100
#define M_MVF 0x0e008100
#define M_RND 0x0e308100
#define M_SIN 0x0e808100
#define M_SQT 0x0e408100
#define M_TAN 0x0ea08100
#define M_FIX 0x0e100110
#define M_FLT 0x0e000110
#define M_WFS 0x0e200110
#define M_RFS 0x0e300110
#define M_WFC 0x0e400110
#define M_RFC 0x0e500110
#define M_CMF 0x0e90f110
#define M_CNF 0x0eb0f110

#define M_URD 0x0ee08100
#define M_NRM 0x0ef08100
#define M_LFM 0x0c100200
#define M_SFM 0x0c000200

#endif
