/*
 * filestack.h
 *
 * (c) Andy Duplain, August 1992.
 */

#ifndef filestack_header_included
#define filestack_header_included

extern long int fileCurrentNo;

int push_file (FILE * fp);
FILE *pop_file (void);
const int get_file (const char **file, long int *line);

#endif
