/* Compatibility functions for the libscl.
   Implementation taken from UnixLib.  */

#include <stdlib.h>

/* Placeholder implementation until we have time to get a proper version.  */
long double strtold (const char *str, char **endptr)
{
    return (long double) strtold (str, endptr);
}


