
/*
 * uname.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifdef UNIX
#include <sys/param.h>		/* for MAXPATHLEN */
#include <unistd.h>		/* for access() stuff */
#define uname(a,b) ((char *)a)
#define DIR '/'
#else
#ifndef UNIXLIB
char *uname (const char *name, int PrefixDir);
#define DIR '.'
#else

#define DIR '/'
#endif
#define MAXPATHLEN 1024
#endif
