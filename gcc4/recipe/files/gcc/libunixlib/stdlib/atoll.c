/* Conversion from string to integer.
   Copyright (c) 2005-2010 UnixLib Devlopers.  */

/* To avoid atoll inlining in stdlib.h resulting in a redefinition: */
#define __NO_INLINE__

#include <stdlib.h>

#undef atoll
long long atoll (const char *s)
{
  return strtoll (s, NULL, 10);
}
