/*
 * help_eval.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef help_eval_header_included
#define help_eval_header_included

#include "value.h"

void help_evalNegLate (Value * value);

void help_evalSubLate (Value * lvalue, const Value * rvalue);

void help_evalAddLate (Value * lvalue, const Value * rvalue);

#endif
