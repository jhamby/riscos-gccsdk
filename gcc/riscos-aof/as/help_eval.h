
/*
 * help_eval.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _help_eval_h
#define _help_eval_h

#ifndef _value_h
#include "value.h"
#endif

void help_evalNegLate (Value * value);

void help_evalSubLate (Value * lvalue, Value * rvalue);

void help_evalAddLate (Value * lvalue, Value * rvalue);

#endif
