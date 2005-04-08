/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdlib/strtold.c,v $
 * $Date: 2002/11/26 21:52:07 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <stdlib.h>

/* Placeholder implementation until we have time to get a proper version.  */
long double strtold (const char *str, char **endptr)
{
    return (long double) strtod (str, endptr);
}


