/*
 * Copyright (c) 2000-2012 UnixLib Developers
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

/* Macros for counting and rounding.  */
#ifndef howmany
# define howmany(x, y)	(((x) + ((y) - 1)) / (y))
#endif
#ifdef __GNUC__
# define rounddown(x, y)	(__builtin_constant_p (y) && powerof2 (y) \
				  ? ((x) & ~((y) - 1)) \
				  : (((x) / (y)) * (y)))
# define roundup(x, y)		(__builtin_constant_p (y) && powerof2 (y) \
				  ? (((x) + (y) - 1) & ~((y) - 1)) \
				  : ((((x) + ((y) - 1)) / (y)) * (y)))
#else
# define rounddown(x, y)	(((x)/(y))*(y))
# define roundup(x, y)		((((x) + ((y) - 1)) / (y)) * (y))
#endif
#define roundup2(x, y)		(((x) + (y) - 1) & ~((y) - 1))
#define powerof2(x)		((((x) - 1) & (x)) == 0)

/* Macros for min/max.  */
#define	MIN(a,b) (((a)<(b))?(a):(b))
#define	MAX(a,b) (((a)>(b))?(a):(b))

#endif
