
/*
 * output.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _output_h
#define _output_h

void outputInit (char *outfile);
void outputFinish (void);
void outputRemove (void);

void outputAof (void);

extern const char *idfn_text;

#if !defined(__riscos__) && defined(WORDS_BIGENDIAN)
unsigned armword (unsigned val);
unsigned ourword (unsigned val);
#else
#define armword(x) (x)
#define ourword(x) (x)
#endif



#endif
