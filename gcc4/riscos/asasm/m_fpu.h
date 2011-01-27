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
 *
 * m_fpu.h
 */

#ifndef m_fpu_header_included
#define m_fpu_header_included

#include <stdbool.h>

#include "global.h"

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

bool c_nofp (void);

#endif
