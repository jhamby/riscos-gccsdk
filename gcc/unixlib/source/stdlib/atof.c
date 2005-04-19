/* Convert string to a double.
   Copyright (c) 2005 UnixLib Developers.  */

#include <stdlib.h>

#undef atof

double atof (const char *string)
{
  return strtod (string, NULL);
}
