/*
 * AsAsm an assembler for ARM
 * Copyright (c) 2010-2014 GCCSDK Developers
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

#ifndef m_cpumem_header_included
#define m_cpumem_header_included

#include <stdbool.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

bool m_clrex (void);
bool m_ldm (bool doLowerCase);
bool m_ldr (bool doLowerCase);
bool m_ldrex (bool doLowerCase);
bool m_pl (bool doLowerCase);
bool m_pop (bool doLowerCase);
bool m_push (bool doLowerCase);
bool m_stm (bool doLowerCase);
bool m_str (bool doLowerCase);
bool m_strex (bool doLowerCase);
bool m_swp (bool doLowerCase);

bool m_dmb (void);
bool m_dsb (void);
bool m_isb (void);

bool m_rfe (bool doLowerCase);
bool m_srs (bool doLowerCase);

bool CPUMem_ConstantInMOVW (uint32_t constant);
bool CPUMem_ConstantInMOVT (uint32_t constant);

#endif
