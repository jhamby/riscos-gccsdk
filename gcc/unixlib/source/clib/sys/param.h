/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/param.h,v $
 * $Date: 2000/07/15 14:52:15 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
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
#define MAXHOSTNAMELEN 256

#endif
