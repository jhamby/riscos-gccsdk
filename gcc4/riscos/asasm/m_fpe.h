/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2011 GCCSDK Developers
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
 */

#ifndef m_fpu_header_included
#define m_fpu_header_included

#include <stdbool.h>

#include "global.h"

#define M_ADF (0x0e000000 | CP_NUMBER (1))
#define M_DVF (0x0e400000 | CP_NUMBER (1))
#define M_FDV (0x0ea00000 | CP_NUMBER (1))
#define M_FML (0x0e900000 | CP_NUMBER (1))
#define M_FRD (0x0eb00000 | CP_NUMBER (1))
#define M_MUF (0x0e100000 | CP_NUMBER (1))
#define M_POL (0x0ec00000 | CP_NUMBER (1))
#define M_POW (0x0e600000 | CP_NUMBER (1))
#define M_RDF (0x0e500000 | CP_NUMBER (1))
#define M_RMF (0x0e800000 | CP_NUMBER (1))
#define M_RPW (0x0e700000 | CP_NUMBER (1))
#define M_RSF (0x0e300000 | CP_NUMBER (1))
#define M_SUF (0x0e200000 | CP_NUMBER (1))

#define M_ABS (0x0e208000 | CP_NUMBER (1))
#define M_ACS (0x0ec08000 | CP_NUMBER (1))
#define M_ASN (0x0eb08000 | CP_NUMBER (1))
#define M_ATN (0x0ed08000 | CP_NUMBER (1))
#define M_COS (0x0e908000 | CP_NUMBER (1))
#define M_EXP (0x0e708000 | CP_NUMBER (1))
#define M_LGN (0x0e608000 | CP_NUMBER (1))
#define M_LOG (0x0e508000 | CP_NUMBER (1))
#define M_MNF (0x0e108000 | CP_NUMBER (1))
#define M_MVF (0x0e008000 | CP_NUMBER (1))
#define M_RND (0x0e308000 | CP_NUMBER (1))
#define M_SIN (0x0e808000 | CP_NUMBER (1))
#define M_SQT (0x0e408000 | CP_NUMBER (1))
#define M_TAN (0x0ea08000 | CP_NUMBER (1))

#define M_FIX (0x0e100010 | CP_NUMBER (1))
#define M_FLT (0x0e000010 | CP_NUMBER (1))
#define M_WFS (0x0e200010 | CP_NUMBER (1))
#define M_RFS (0x0e300010 | CP_NUMBER (1))
#define M_WFC (0x0e400010 | CP_NUMBER (1))
#define M_RFC (0x0e500010 | CP_NUMBER (1))
#define M_CMF (0x0e90f010 | CP_NUMBER (1))
#define M_CNF (0x0eb0f010 | CP_NUMBER (1))

#define M_URD (0x0ee08000 | CP_NUMBER (1))
#define M_NRM (0x0ef08000 | CP_NUMBER (1))
#define M_LDF (0x0c000000 | CP_NUMBER (1) | L_FLAG)
#define M_STF (0x0c000000 | CP_NUMBER (1))

#define M_LFM (0x0c000000 | CP_NUMBER (2) | L_FLAG)
#define M_SFM (0x0c000000 | CP_NUMBER (2))

#define M_FMNEM 0x0ef08110

bool m_abs (void);
bool m_acs (void);
bool m_adf (void);
bool m_asn (void);
bool m_atn (void);
bool m_cmf (void);
bool m_cnf (void);
bool m_cos (void);
bool m_dvf (void);
bool m_exp (void);
bool m_fdv (void);
bool m_fix (void);
bool m_flt (void);
bool m_fml (void);
bool m_frd (void);
bool m_lgn (void);
bool m_log (void);
bool m_mnf (void);
bool m_muf (void);
bool m_mvf (void);
bool m_nrm (void);
bool m_pol (void);
bool m_pow (void);
bool m_rdf (void);
bool m_rfc (void);
bool m_rfs (void);
bool m_rmf (void);
bool m_rnd (void);
bool m_rpw (void);
bool m_rsf (void);
bool m_sin (void);
bool m_sqt (void);
bool m_suf (void);
bool m_tan (void);
bool m_urd (void);
bool m_wfs (void);
bool m_wfc (void);

bool m_ldf (void);
bool m_lfm (void);
bool m_sfm (void);
bool m_stf (void);

bool c_nofp (void);

ARMWord FPE_ConvertImmediate (ARMFloat d);

#endif
