
/*
 * eval.c
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _evel_h
#define _evel_h

#ifndef _global_h
#include "global.h"
#endif

#ifndef _lex_h
#include "lex.h"
#endif

#ifndef _value_h
#include "value.h"
#endif


BOOL evalBinop (Operator op, Value * lvalue, Value * rvalue);
BOOL evalUnop (Operator op, Value * value);

#endif
