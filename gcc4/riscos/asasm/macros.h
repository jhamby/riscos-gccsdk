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
 * macros.h
 */

#ifndef macros_header_included
#define macros_header_included

#include <stdbool.h>
#include "global.h"
#include "lex.h"
#include "variables.h"
#include "whileif.h"

#define MACRO_ARG_LIMIT (16)
/* These delimit a block of 16 consecutive character codes which cannot
 * appear in the assembler source
 */
#define MACRO_ARG0  16
#define MACRO_ARG15 31

typedef struct Macro
{
  const struct Macro *next;
  const char *name;
  const char *file;
  const char *buf;
  unsigned int labelarg:8,	/* 0 or 1 of these */
    numargs:8;			/* and up to 16 in total */
  char *args[MACRO_ARG_LIMIT];
  int startline;
} Macro;

typedef struct
{
  const Macro *macro;
  const char *curPtr; /**< Current pointer inside macro buffer Macro::buf.  */
  const char *args[MACRO_ARG_LIMIT];
  varPos *varListP; /**< Linked list of local variables defined in this macro.  */
} MacroPObject;

void FS_PopMacroPObject (bool noCheck);

const Macro *macroFind (const char *, size_t len);
void macroCall (const Macro *, const Lex *);
bool macroGetLine (char *buf);	/* returns 0 if end of macro */

bool c_macro (const Lex *);
bool c_mexit (const Lex *);

#endif
