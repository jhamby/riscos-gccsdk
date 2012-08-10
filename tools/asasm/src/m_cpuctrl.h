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

bool m_adr (bool doLowerCase);
bool m_bkpt (void);
bool m_blx (bool doLowerCase);
bool m_branch (bool doLowerCase);
bool m_bx (bool doLowerCase);
bool m_bxj (bool doLowerCase);
bool m_mrs (bool doLowerCase);
bool m_msr (bool doLowerCase);
bool m_swi (bool doLowerCase);

bool m_sev (bool doLowerCase);
bool m_wfe (bool doLowerCase);
bool m_wfi (bool doLowerCase);
bool m_yield (bool doLowerCase);

bool m_cps (bool doLowerCase);
bool m_dbg (bool doLowerCase);
bool m_smc (bool doLowerCase);

#endif
