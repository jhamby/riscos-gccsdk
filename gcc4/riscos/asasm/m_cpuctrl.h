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
 * m_cpuctrl.h
 */

#ifndef m_cpuctrl_header_included
#define m_cpuctrl_header_included

#include <stdbool.h>

#include "decode.h"

Rslt_e m_adr (bool doLowerCase);
Rslt_e m_bkpt (void);
Rslt_e m_blx (bool doLowerCase);
Rslt_e m_branch (bool doLowerCase);
Rslt_e m_bx (bool doLowerCase);
Rslt_e m_bxj (bool doLowerCase);
Rslt_e m_mrs (bool doLowerCase);
Rslt_e m_msr (bool doLowerCase);
Rslt_e m_swi (bool doLowerCase);

Rslt_e m_sev (bool doLowerCase);
Rslt_e m_wfe (bool doLowerCase);
Rslt_e m_wfi (bool doLowerCase);
Rslt_e m_yield (bool doLowerCase);

Rslt_e m_cps (bool doLowerCase);
Rslt_e m_dbg (bool doLowerCase);
Rslt_e m_smc (bool doLowerCase);

#endif
