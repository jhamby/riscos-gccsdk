/*
 * output.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef output_header_included
#define output_header_included

void outputInit (char *outfile);
void outputFinish (void);
void outputRemove (void);

void outputAof (void);
void outputElf (void);

extern const char *idfn_text;

#if defined(WORDS_BIGENDIAN)
unsigned armword (unsigned val);
unsigned ourword (unsigned val);
#else
#define armword(x) (x)
#define ourword(x) (x)
#endif

#endif
