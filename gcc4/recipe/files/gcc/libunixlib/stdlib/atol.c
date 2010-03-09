/* Conversion from string to integer.
   Copyright (c) 2005-2010 UnixLib Devlopers.  */

/* To avoid atol inlining in stdlib.h resulting in a redefinition: */
#define __NO_INLINE__

#include <stdlib.h>

#undef atol
long atol (const char *s)
{
  return strtol (s, NULL, 10);
}
