/*
 * depend.h
 * Copyright © 1997 Darren Salt
 * Copyright © 2005 GCCSDK Developers
 */

#ifndef depend_header_included
#define depend_header_included

#include <stdio.h>

extern const char *DependFileName;

void dependInit (const char *srcFileName);
void dependFinish (void);

void dependPut (const char *pre, const char *name, const char *post);
void dependOpen (const char *objname);

#define dependWrite(c) dependPut(" ",(c),"")

#endif
