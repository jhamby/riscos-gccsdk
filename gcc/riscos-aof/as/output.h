/*
 * output.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef output_header_included
#define output_header_included

#include "global.h"

void outputInit (const char *outfile);
void outputFinish (void);
void outputRemove (void);

void outputAof (void);
void outputElf (void);

extern const char *idfn_text;

#if defined(WORDS_BIGENDIAN)
WORD armword (WORD val);
WORD ourword (WORD val);
#else
#define armword(x) (x)
#define ourword(x) (x)
#endif

#endif
