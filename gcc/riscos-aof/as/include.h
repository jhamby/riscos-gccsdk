/*
 * include.h
 *
 * (c) Andy Duplain, August 1992.
 */

#ifndef include_header_included
#define include_header_included

int initInclude (void);
int addInclude (const char *incpath);
FILE *getInclude (const char *filename, const char *mode);

#endif
