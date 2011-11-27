/*
 * AS an assembler for ARM
 * Copyright (c) August 1992, Andy Duplain
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
 * include.h
 */

#ifndef include_header_included
#define include_header_included

#include <stdbool.h>
#include <stdio.h>

#include "os.h"

void Include_Add (const char *incpath);
bool Include_Find (const char *filename, ASFile *asFileP, bool inc);
FILE *Include_Get (const char *filename, ASFile *asFileP, bool inc);

#endif
