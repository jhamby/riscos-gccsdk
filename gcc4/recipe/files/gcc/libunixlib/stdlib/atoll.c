/* Conversion from string to integer.
   Copyright (c) 2005 UnixLib Devlopers.  */

#include <stdlib.h>

#undef atoll
long long atoll (const char *s)
{
  return strtoll (s, (char **) NULL, 10);
}
