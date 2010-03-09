/* Convert string to a double.
   Copyright (c) 2005-2010 UnixLib Developers.  */

/* To avoid atof inlining in stdlib.h resulting in a redefinition: */
#define __NO_INLINE__

#include <stdlib.h>

#undef atof
double atof (const char *string)
{
  return strtod (string, NULL);
}
