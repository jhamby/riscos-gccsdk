/*
 * help_lex.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef help_lex_header_included
#define help_lex_header_included

#include "global.h"

WORD lexGetCharFromString (int *ilen, const char **istr);
WORD lexChar2Int (BOOL rev, int len, const char *str);

#endif
