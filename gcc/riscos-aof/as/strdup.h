/*
 *  strdup.h
 * Copyright © 1997 Darren Salt
 */

#ifndef _strdup_h
#define _strdup_h

#include <stdlib.h>

#if defined(UNIXLIB) || defined(CROSS_COMPILE)
#include <string.h>
#include "error.h"

#else

#include "error.h"

#if !(defined(UNIX) && defined(USG))
#if !defined(__GNUC_MINOR__) || __GNUC__<2 || __GNUC_MINOR__<7
char *strdup (char *);
#endif
#endif
#endif

char *strndup (const char *, int len);

#endif
