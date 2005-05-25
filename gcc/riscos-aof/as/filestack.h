/*
 * filestack.h
 *
 * (c) Andy Duplain, August 1992.
 * Copyright © 2005 GCCSDK Developers
 */

#ifndef filestack_header_included
#define filestack_header_included

int push_file (FILE * fp);
FILE *pop_file (void);
int get_file (const char **file, long int *line);

#endif
