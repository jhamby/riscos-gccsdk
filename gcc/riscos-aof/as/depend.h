/*
 * depend.h
 * Copyright � 1997 Darren Salt
 */

#ifndef _depend_h
#define _depend_h

#include <stdio.h>

extern FILE *dependfile;
extern const char *DependFileName;

void dependPut (const char *pre, const char *name, const char *post);
void dependOpen (const char *objname);

#define dependWrite(c) dependPut(" ",(c),"")

#endif
