
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

extern char *idfn_text;

#endif
