/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/timegm.c,v $
 * $Date: 2004/10/08 11:56:13 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

/* POSIX equivalent function - take from timegm man page under Linux  */

#include <time.h>
#include <stdlib.h>

time_t
timegm (struct tm *tm) {
    time_t ret;
    char *tz;

    tz = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();
    ret = mktime(tm);
    if (tz)
        setenv("TZ", tz, 1);
    else
        unsetenv("TZ");
    tzset();
    return ret;
}

