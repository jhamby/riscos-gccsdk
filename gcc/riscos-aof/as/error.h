/*
 * error.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef error_header_included
#define error_header_included

#include "global.h"

extern char er[1024];

void errorInit (char *filename);

#ifdef __riscos__
void errorFinish (void);
#else
#define errorFinish()
#endif

typedef enum
{
  ErrorInfo,
  ErrorWarning,
  ErrorError,
  ErrorSerious,
  ErrorAbort
}
ErrorTag;

void errorLine (long int lineno, const char *file,
		ErrorTag e, BOOL c, const char *format, ...);
void error (ErrorTag e, BOOL c, const char *format, ...);
void errorOutOfMem (const char *fn);

int noerrors (void);
int nowarnings (void);

extern const char *const InsertCommaAfter;

#endif
