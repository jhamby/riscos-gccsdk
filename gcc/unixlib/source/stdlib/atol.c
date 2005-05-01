/* Conversion from string to integer.
   Copyright (c) 2005 UnixLib Devlopers.  */

#include <stdlib.h>

#undef atol
long atol (const char *s)
{
  return strtol (s, (char **) NULL, 10);
}
