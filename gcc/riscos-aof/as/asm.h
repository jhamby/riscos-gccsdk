
/*
 *  asm.h
 * Copyright � 1992 Niklas R�jemo
 */

#ifndef _lex_h
#include "lex.h"
#endif
#ifndef _symbol_h
#include "symbol.h"
#endif

extern void asm_ (void);
extern Symbol *asm_label (Lex * label);
