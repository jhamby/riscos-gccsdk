/*
 * eval.c
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef eval_header_included
#define eval_header_included

#include "global.h"
#include "lex.h"
#include "value.h"

BOOL evalBinop (Operator op, Value * lvalue, const Value * rvalue);
BOOL evalUnop (Operator op, Value * value);

#endif
