/*
 * decode.h
 * Copyright � 1992 Niklas R�jemo
 */

#ifndef decode_header_included
#define decode_header_included

#include "symbol.h"

extern int returnvalue;

BOOL notinput (const char *str);
int decode (Lex * label);

#endif
