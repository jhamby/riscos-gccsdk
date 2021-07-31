/* Conversion from string to integer.
   Copyright (c) 2005 UnixLib Devlopers.  */

#include <stdlib.h>

#undef atoi
int atoi (const char *s)
{
  return (int) strtol (s, (char **) NULL, 10);
}
