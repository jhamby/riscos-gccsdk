/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/param,v $
 * $Date: 2000/06/03 14:28:30 $
 * $Revision: 1.7 $
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
#define MAXHOSTNAMELEN 256

#endif
