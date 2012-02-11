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
 * m_cpumem.h
 */

#ifndef m_cpumem_header_included
#define m_cpumem_header_included

#include <stdbool.h>

#include "decode.h"

Rslt_e m_clrex (void);
Rslt_e m_ldm (bool doLowerCase);
Rslt_e m_ldr (bool doLowerCase);
Rslt_e m_ldrex (bool doLowerCase);
Rslt_e m_pl (bool doLowerCase);
Rslt_e m_pop (bool doLowerCase);
Rslt_e m_push (bool doLowerCase);
Rslt_e m_stm (bool doLowerCase);
Rslt_e m_str (bool doLowerCase);
Rslt_e m_strex (bool doLowerCase);
Rslt_e m_swp (bool doLowerCase);

Rslt_e m_dmb (void);
Rslt_e m_dsb (void);
Rslt_e m_isb (void);

Rslt_e m_rfe (bool doLowerCase);
Rslt_e m_srs (bool doLowerCase);

#endif
