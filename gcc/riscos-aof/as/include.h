/*
 * include.h
 *
 * (c) Andy Duplain, August 1992.
 */

#ifndef __INCLUDE_H
#define __INCLUDE_H

int initInclude (void);
int addInclude (const char *incpath);
FILE *getInclude (const char *filename, const char *mode);

#endif /* __INCLUDE_H */
