/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2006 GCCSDK Developers
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
 * input.h
 */

#ifndef input_header_included
#define input_header_included

#include <ctype.h>
#include <stdio.h>

#include "global.h"
#include "macros.h"

#define FLIP(x) (isupper((x)) ? tolower((x)) : islower((x)) ? toupper((x)) : ((x)))
#define TOLOWER(x) tolower((x))

extern FILE *asmfile;
extern long int inputLineNo;	/* Do not change, only read! */
extern BOOL inputExpand;
extern BOOL inputRewind;
extern const char *inputName;

void inputInit (const char *infile);
void inputFinish (FILE *newAsmFile);

/* Debug only:
 */
const char *inputGiveRestLine(void);

BOOL inputNextLine (void);

void skipblanks (void);
void skiprest (void);

BOOL inputComment (void);
char inputLook (void);
char inputLookLower (void);
char inputLookUC (void);
char inputLookN (int n);
char inputLookNLower (int n);
char inputLookNUC (int n);
char inputGet (void);
char inputGetLower (void);
char inputGetUC (void);
void inputUnGet (char c);
void inputPutBack (char c);
char inputSkipLook (void);
char *inputRest (void);
char *inputLine (void);
void inputSkip (void);
void inputSkipN (int n);
char *inputSymbol (int *ilen, char del);

void inputMark (void);
void inputRollback (void);

void inputThisInstead (const char *);

#endif
