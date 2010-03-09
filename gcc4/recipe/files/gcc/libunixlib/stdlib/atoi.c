/* Conversion from string to integer.
   Copyright (c) 2005-2010 UnixLib Devlopers.  */

/* To avoid atoi inlining in stdlib.h resulting in a redefinition: */
#define __NO_INLINE__

#include <stdlib.h>

#undef atoi
int atoi (const char *s)
{
  return (int) strtol (s, NULL, 10);
}
