/*
 * decode.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef decode_header_included
#define decode_header_included

#include "symbol.h"

extern int returnvalue;

BOOL notinput (const char *str);
int decode (Lex * label);

#endif
