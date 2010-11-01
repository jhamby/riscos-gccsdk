/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2006 GCCSDK Developers
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

#include "symbol.h"

void c_align (void);
void c_area (void);
void c_assert (void);
void c_info (void);

void c_rn (Symbol * symbol);
void c_fn (Symbol * symbol);
void c_cn (Symbol * symbol);
void c_cp (Symbol * symbol);
void c_equ (Symbol * symbol);

void c_reserve (void);

void c_ltorg (void);

void c_head (void);		/* fn name (prior to fn entry code) */

void c_dcb (void);		/*  8 bits int */
void c_dcw (void);		/* 16 bits int */
void c_dcd (void);		/* 32 bits int */
void c_dcfs (void);		/* 32 bits float */
void c_dcfd (void);		/* 64 bits float */
void c_dcs (void);		/* asci string */
void c_dcz (void);		/* zero-terminated ascii string */
void c_entry (void);
void c_globl (void);
void c_import (void);
void c_strong (void);
void c_keep (void);
void c_get (void);		/* get <filename> */
void c_lnk (void);		/* lnk <filename> */
void c_idfn (void);		/* idfn <text> */
void c_bin (void);
void c_opt (void);

void c_title (void);

void c_end (void);

#endif
