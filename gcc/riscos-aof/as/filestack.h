/*
 * filestack.h
 *
 * (c) Andy Duplain, August 1992.
 */

#ifndef __FILESTACK_H
#define __FILESTACK_H

int push_file (FILE * fp);
FILE *pop_file (void);
const int get_file (const char **file, long int *line);

#endif /* __FILESTACK_H */
