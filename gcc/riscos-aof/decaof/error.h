/*
 * error/warning report function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __ERROR_H
#define __ERROR_H

#include <stdarg.h>

void error (char *fmt, ...);
void warning (char *fmt, ...);

#endif /* __ERROR_H */
