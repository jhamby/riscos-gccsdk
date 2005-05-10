/*
 * error/warning report function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 * Copyright 2005 GCCSDK Developers
 */

#ifndef ERROR_HEADER_INCLUDED
#define ERROR_HEADER_INCLUDED

#include <stdarg.h>

void error (char *fmt, ...);
void warning (char *fmt, ...);

#endif /* ERROR_HEADER_INCLUDED */
