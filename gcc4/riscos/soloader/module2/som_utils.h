#ifndef SOM_UTILS_H
#define SOM_UTILS_H

#include "som_types.h"

/* Compare two strings where s2 may be wildcarded. If lower is true
   then disregard case.
   Return TRUE if a match is found.  */
bool
wildcmp(const char *s1,
	const char *s2,
	bool lower);

/* Make a C copy of an argument string that may be terminated by any control character.
 * SOM_Alloc is used for memory allocation, the caller is responsible for freeing it.  */
char *copy_arg_string(const char *arg_string);

#endif
