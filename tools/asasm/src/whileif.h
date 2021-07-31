/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1997 Darren Salt
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef whileif_header_included
#define whileif_header_included

#include <stdbool.h>

bool c_if (void);
bool c_else (void);
bool c_elif (void);
bool c_endif (void);

bool c_while (void);
bool c_wend (void);

void FS_PopIfWhile (bool noCheck);

#endif
