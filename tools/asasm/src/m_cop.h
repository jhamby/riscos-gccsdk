/*
 * AS an assembler for ARM
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * m_cop.h
 */

#ifndef m_cop_header_included
#define m_cop_header_included

#include <stdbool.h>

#include "decode.h"

bool m_cdp (bool doLowerCase);
bool m_mcr (bool doLowerCase);
bool m_mcrr (bool doLowerCase);
bool m_mrc (bool doLowerCase);
bool m_mrrc (bool doLowerCase);

#endif
