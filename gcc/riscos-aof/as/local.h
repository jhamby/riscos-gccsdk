/*
 * local.h
 * Copyright © 1997 Darren Salt
 */

#ifndef _local_h
#define _local_h

#ifndef _lex_h
#include "lex.h"
#endif

extern int rout_lblno[100];
extern char *rout_id;

extern const char localFormat[];
extern int localCurrent;

void c_rout (Lex * label);
void c_local (Lex * label);

int localTest (const char *);
void localMunge (Lex *);
void localFindLocal (int local, const char **file, long int *lineno);
void localFindRout (const char *rout, const char **file, long int *lineno);

#endif
