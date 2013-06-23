/*
 * AsAsm an assembler for ARM
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef m_cpuctrl_header_included
#define m_cpuctrl_header_included

#include <stdbool.h>

bool m_adr (bool doLowerCase);
bool m_bkpt (bool doLowerCase);
bool m_blx (bool doLowerCase);
bool m_branch (bool doLowerCase);
bool m_bx (bool doLowerCase);
bool m_bxj (bool doLowerCase);
bool m_cbnz_cbz (bool doLowerCase);
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
bool m_smi (bool doLowerCase);

#endif
