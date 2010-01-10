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

#include "global.h"
#include "lex.h"
#include "whileif.h"

#define MACRO_ARG0  16
#define MACRO_ARG15 31
/* These delimit a block of 16 consecutive character codes which cannot
 * appear in the assembler source
 */

typedef struct Macro
{
  const struct Macro *next;
  char *name;
  const char *file;
  char *buf;
  unsigned int labelarg:8,	/* 0 or 1 of these */
    numargs:8;			/* and up to 16 in total */
  char *args[16];
  long int startline;
}
Macro;

typedef struct MacroStack
{
  const Macro *macro;
  long int callno;
  const char *offset;		/* Filled in for *current* macro */
  long int lineno;		/* Return to this line... */
  WhileBlock *whilestack;
  int if_depth;
  char *args[16];
}
MacroStack;

extern int macroSP;
extern MacroStack macroStack[10];
extern char *macroArgs[16];
extern const Macro *macroCurrent;
extern const char *macroPtr;
extern long int macroCurrentCallNo;

const Macro *macroFind (size_t len, const char *);
void macroCall (const Macro *, Lex *);
BOOL macroGetLine (char *buf);	/* returns 0 if end of macro */

BOOL macroAdd (Macro *);

void c_macro (Lex *);		/* start of macro definition */
void c_mexit (Lex *);

#endif
