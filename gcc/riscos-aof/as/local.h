/*
 * local.h
 * Copyright © 1997 Darren Salt
 */

#ifndef local_header_included
#define local_header_included

#include "lex.h"

extern int rout_lblno[100];
extern const char *rout_id;

extern const char localFormat[];
extern int localCurrent;

void c_rout (Lex * label);
void c_local (Lex * label);

int localTest (const char *);
void localMunge (Lex *);
void localFindLocal (int local, const char **file, long int *lineno);
void localFindRout (const char *rout, const char **file, long int *lineno);

#endif
