/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2012 GCCSDK Developers
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

#include <stdbool.h>

#include "decode.h"
#include "global.h"
#include "value.h"

Rslt_e c_align (void);
Rslt_e c_ampersand (void);
Rslt_e c_data (void);
Rslt_e c_dcb (void);
Rslt_e c_dcw (bool doLowerCase);
Rslt_e c_dcd (bool doLowerCase);
Rslt_e c_dci (void);
Rslt_e c_dcfd (bool doLowerCase);
Rslt_e c_dcfs (bool doLowerCase);
Rslt_e c_fill (void);
Rslt_e c_reserve (void);

typedef struct
{
  int size; /**< Size of the data unit : 1, 2 or 4.  */
  bool allowUnaligned; /**< Allow unaligned data storage.  */
} DefineInt_PrivData_t;

typedef struct
{
  int size; /**< Size of the data unit : 4 or 8.  */
  bool allowUnaligned; /**< Allow unaligned data storage.  */
} DefineReal_PrivData_t;

bool DefineInt_RelocUpdater (const char *fileName, unsigned lineNum, ARMWord offset,
			     const Value *valueP, void *privData, bool final);
bool DefineReal_RelocUpdater (const char *fileName, unsigned lineNum, ARMWord offset,
			      const Value *valueP, void *privData, bool final);

#endif
