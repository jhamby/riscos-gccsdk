/*
 * AS an assembler for ARM
 * Copyright (c) 2010-2011 GCCSDK Developers
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

bool m_adr (void);
bool m_bkpt (void);
bool m_blx (void);
bool m_branch (void);
bool m_bx (void);
bool m_bxj (void);
bool m_mrs (void);
bool m_msr (void);
bool m_ret (void);
bool m_stack (void);
bool m_swi (void);
bool m_tail (void);

bool m_sev (void);
bool m_wfe (void);
bool m_wfi (void);
bool m_yield (void);

bool m_cps (void);
bool m_dbg (void);
bool m_smc (void);

#endif
