/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/stdarg,v $
 * $Date: 1997/10/09 19:59:48 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
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
	*(t *)(((a) += va_align(sizeof(t))) - va_align(sizeof(t))) : \
	(t)(*(int *)(((a) += sizeof(int)) - sizeof(int))))
#define va_end(a)	((void)((a) = (char *)-1))

#ifdef __cplusplus
	}
#endif

#endif
