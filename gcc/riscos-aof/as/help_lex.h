/*
 * help_lex.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _help_lex_h
#define _help_lex_h

#ifndef _global_h
#include "global.h"
#endif

WORD lexGetCharFromString (int *ilen, char **istr);
WORD lexChar2Int (BOOL rev, int len, char *str);

#endif
