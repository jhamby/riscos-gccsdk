/*
 *  asm.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef asm_header_included
#define asm_header_included

#include "lex.h"
#include "symbol.h"

extern void assemble (void);
extern Symbol *asm_label (Lex * label);

#endif
