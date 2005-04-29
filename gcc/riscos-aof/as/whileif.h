/*
 * AS an assembler for ARM
 * Copyright � 1997 Darren Salt
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

#include "symbol.h"

typedef enum
{
  WhileNull = 0,
  WhileInFile = 1,
  WhileInMacro = 2
}
WhileTag;

typedef struct WhileBlock
{
  struct WhileBlock *prev;
  WhileTag tag;
  const char *expr;	/* Ptr to malloced block holding the expression */
  union
  {
    struct
    {
      long int offset;
    }
    file;		/* Valid when tag == WhileInFile */
    struct
    {
      long int callno;
      const char *offset;
    }
    macro;		/* Valid when tag == WhileInMacro */
  }
  ptr;
  long int lineno;
}
WhileBlock;

extern int if_depth;
extern WhileBlock *whileCurrent;

void c_if (void);
void c_else (Lex *);
void c_endif (Lex *);

void c_while (Lex *);
void c_wend (Lex *);

void testUnmatched (void);

#endif
