/*
 * error.h
 * Copyright © 1992 Niklas Röjemo
 * Copyright © 2005 GCCSDK Developers
 */

#ifndef error_header_included
#define error_header_included

#include "global.h"

typedef enum
{
  ErrorInfo,
  ErrorWarning,
  ErrorError,
  ErrorSerious,
  ErrorAbort
}
ErrorTag;

extern const char *const InsertCommaAfter;

void errorInit (const char *filename);
void errorFinish (void);
void errorLine (long int lineno, const char *file,
		ErrorTag e, BOOL c, const char *format, ...);
void error (ErrorTag e, BOOL c, const char *format, ...);
void errorOutOfMem (const char *fn);

int returnExitStatus (void);

#endif
