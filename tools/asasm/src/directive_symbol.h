/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2014 GCCSDK Developers
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

#ifndef directive_symbol_header_included
#define directive_symbol_header_included

#include <stdbool.h>

struct Symbol;

bool c_cn (struct Symbol *symbol);
bool c_cp (struct Symbol *symbol);
/* FIXME: bool c_dn (struct Symbol *symbol); */
bool c_equ (struct Symbol *symbol);
bool c_fn (struct Symbol *symbol);
/* FIXME: bool c_qn (struct Symbol *symbol); */
bool c_rlist (struct Symbol *symbol);
bool c_rn (struct Symbol *symbol);
/* FIXME: bool c_sn (struct Symbol *symbol); */

#endif
