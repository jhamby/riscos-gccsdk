/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/param.h,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __SYS_PARAM_H
#define __SYS_PARAM_H

#ifndef __LIMITS_H
#include <limits.h>
#endif

#define MAXPATHLEN	_POSIX_PATH_MAX

/* Of the Unix systems I inspected, only Solaris puts maxhostnamelen in
 *<netdb.h>
 */
/*
 * Maximum length of a hostname
 */
#ifndef MAXHOSTNAMELEN
#  define MAXHOSTNAMELEN 256
#endif

#endif
