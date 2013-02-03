/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2013 GCCSDK Developers
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
 * directive_data.h
 */

#ifndef directive_data_header_included
#define directive_data_header_included

#include "config.h"

#include <stdbool.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "value.h"

bool c_align (void);
bool c_ampersand (void);
bool c_data (void);
bool c_dcb (void);
bool c_dcw (bool doLowerCase);
bool c_dcd (bool doLowerCase);
bool c_dci (bool doLowerCase);
bool c_dcfd (bool doLowerCase);
bool c_dcfh (bool doLowerCase);
bool c_dcfs (bool doLowerCase);
bool c_dcq (bool doLowerCase);
bool c_fill (void);
bool c_reserve (void);

bool DefineInt_HandleSymbols (unsigned size, bool allowUnaligned,
			      bool swapHalfwords, uint32_t offset,
			      const Value *valueP);

#endif
