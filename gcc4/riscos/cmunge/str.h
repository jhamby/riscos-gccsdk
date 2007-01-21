/*
 * Copyright (C) 1999 Robin Watts/Justin Fletcher
 */

#ifndef STR_H
#define STR_H

int stricmp(const char *p, const char *q);

char *strdup_strip(const char *p);

const char *strduptok(const char *p, char **res);

int strprefix(const char *p, const char *q);

const char *strfindpair(const char *p, char **p1, char **p2);

const char *strcomma(const char *s);

const char *strskip(const char *s, char c);

const char *strstring(const char *s, char **res);

const char *strint(const char *s, unsigned int *i);

#endif
