
/*
 * commands.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _variables_h
#define _variables_h

#include "symbol.h"

void c_gbl (ValueTag, Lex *);	/* global variable declaration */
void c_lcl (ValueTag, Lex *);	/* local variable declaration */
void c_set (ValueTag, Lex *);	/* variable assignment */

void var_restoreLocals (void);	/* called on macro exit */
void var_define (const char *);

#endif
