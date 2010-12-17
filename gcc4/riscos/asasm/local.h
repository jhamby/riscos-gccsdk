/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2002-2010 GCCSDK Developers
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
 * local.h
 */

#ifndef local_header_included
#define local_header_included

#include <stdbool.h>
#include "lex.h"

extern int Local_ROUTLblNo[100];
extern const char *Local_CurROUTId;

extern const char Local_IntLabelFormat[];

bool c_rout (const Lex *label);

bool Local_ROUTIsEmpty (const char *routName);
bool Local_IsLocalLabel (const char *);
void Local_FindROUT (const char *rout, const char **file, int *lineno);

#endif
