/* strtold ()
 * Copyright (c) 2002-2006 UnixLib Developers
 */

#include <stdlib.h>

/* Placeholder implementation until we have time to get a proper version.  */
long double strtold (const char *str, char **endptr)
{
    return (long double) strtod (str, endptr);
}
