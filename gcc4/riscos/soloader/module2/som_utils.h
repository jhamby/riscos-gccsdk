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

#endif
