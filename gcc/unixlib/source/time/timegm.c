/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/mktime.c,v $
 * $Date: 2002/02/14 15:56:38 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mktime.c,v 1.4 2002/02/14 15:56:38 admin Exp $";
#endif

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

