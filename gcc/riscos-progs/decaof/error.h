/*
 * error/warning report function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __ERROR_H
#define __ERROR_H

#include "cproto.h"

#ifdef ANSI_C
#include "stdarg.h"
#else
#include "varargs.h"
#endif

void error P__((char *fmt, ...));
void warning P__((char *fmt, ...));

#endif /* __ERROR_H */
