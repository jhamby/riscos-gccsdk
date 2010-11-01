/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2004-2010 GCCSDK Developers
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
 * whileif.h
 */

#ifndef whileif_header_included
#define whileif_header_included

#include <stdbool.h>
#include "symbol.h"

typedef enum
{
  WhileNull = 0,
  WhileInFile = 1,
  WhileInMacro = 2
} WhileTag;

typedef struct WhileBlock
{
  struct WhileBlock *prev;
  WhileTag tag;
  const char *expr;	/* Ptr to malloced block holding the expression */
  union
    {
      struct
	{
	  int offset;
	} file;		/* Valid when tag == WhileInFile */
      struct
	{
	  const char *offset;
	} macro;	/* Valid when tag == WhileInMacro */
    } ptr;
  int lineno;
} WhileBlock;

void c_if (const Lex *);
void c_else (const Lex *);
void c_endif (const Lex *);

void c_while (const Lex *);
void c_wend (const Lex *);

void FS_PopIfWhile (bool noCheck);

#endif
