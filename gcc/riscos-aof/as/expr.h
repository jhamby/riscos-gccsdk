
/*
 * expr.h
 * Copyright � 1992 Niklas R�jemo
 */

#ifndef _expr_h
#define _expr_h

#ifndef _value_h
#include "value.h"
#endif

void exprBuild (void);		/* Parse the input */
Value exprEval (ValueTag legal);

#endif
