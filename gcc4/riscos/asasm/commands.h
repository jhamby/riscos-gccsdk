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
 * commands.h
 */

#ifndef commands_header_included
#define commands_header_included

#include <stdbool.h>
#include "symbol.h"

bool c_ampersand (void);
bool c_assert (void);
bool c_cn (Symbol *symbol);
bool c_cp (Symbol *symbol);
bool c_data (void);
bool c_dcb (void);
bool c_dcw (bool doLowerCase);
bool c_dcd (bool doLowerCase);
bool c_dci (void);
bool c_dcfd (bool doLowerCase);
bool c_dcfs (bool doLowerCase);
bool c_end (void);
bool c_equ (Symbol *symbol);
bool c_fn (Symbol *symbol);
bool c_get (void);
bool c_idfn (void);
bool c_incbin (void);
bool c_info (void);
bool c_lnk (void);
bool c_opt (void);
bool c_rn (Symbol *symbol);
bool c_title (void);

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

bool DefineInt_RelocUpdater (const char *file, int lineno, ARMWord offset,
			     const Value *valueP, void *privData, bool final);
bool DefineReal_RelocUpdater (const char *file, int lineno, ARMWord offset,
			      const Value *valueP, void *privData, bool final);

#endif
