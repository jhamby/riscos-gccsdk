/*
 * expr.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef expr_header_included
#define expr_header_included

#include "value.h"

void exprBuild (void);		/* Parse the input */
Value exprEval (ValueTag legal);

#endif
