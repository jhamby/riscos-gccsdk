/* Compatibility functions for the libscl.
   Implementation taken from UnixLib.  */

#include <stdlib.h>

/* Placeholder implementation until we have time to get a proper version.  */
float strtof (const char *str, char **endptr)
{
    return (float) strtod (str, endptr);
}
