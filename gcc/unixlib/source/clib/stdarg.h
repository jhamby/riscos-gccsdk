/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/stdarg.h,v $
 * $Date: 2002/02/07 10:19:30 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __STDARG_H
#define __STDARG_H
#define __VARARGS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char *va_list;

#define va_align(x)	(((x) + (sizeof(int) - 1)) & (~(sizeof(int) - 1)))

#define va_start(a,p)	((void)((a) = ((char *)(&(p)) + va_align(sizeof(p)))))
#define va_arg(a,t)	((sizeof(t) > sizeof(int)) ? \
	*(t *)(void *)(((a) += va_align(sizeof(t))) - va_align(sizeof(t))) : \
	(t)(*(int *)(void *)(((a) += sizeof(int)) - sizeof(int))))
#define va_end(a)	((void)((a) = (char *)-1))
#define va_copy(dest, src) (dest) = (src)

#ifdef __cplusplus
	}
#endif

#endif
