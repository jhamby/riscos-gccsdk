/*
 * filestack.h
 *
 * Copyright (c) Andy Duplain, August 1992.
 * Copyright (c) 2004-2006 GCCSDK Developers
 */

#ifndef filestack_header_included
#define filestack_header_included

extern int push_file (FILE * fp);
extern FILE *pop_file (void);
extern int get_file (const char **file, long int *line);

#endif
