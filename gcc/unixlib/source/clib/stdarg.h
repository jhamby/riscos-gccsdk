/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifndef __STDARG_H
#define __STDARG_H
#define __VARARGS_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

__BEGIN_DECLS

typedef char *va_list;

#define va_align(x)	(((x) + (sizeof(int) - 1)) & (~(sizeof(int) - 1)))

#define va_start(a,p)	((void)((a) = ((char *)(&(p)) + va_align(sizeof(p)))))
#define va_arg(a,t)	((sizeof(t) > sizeof(int)) ? \
	*(t *)(void *)(((a) += va_align(sizeof(t))) - va_align(sizeof(t))) : \
	(t)(*(int *)(void *)(((a) += sizeof(int)) - sizeof(int))))
#define va_end(a)	((void)((a) = (char *)-1))
#define va_copy(dest, src) (dest) = (src)

__END_DECLS

#endif
