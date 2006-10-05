/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
