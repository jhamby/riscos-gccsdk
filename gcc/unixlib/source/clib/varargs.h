/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/varargs,v $
 * $Date: 1997/10/09 19:59:50 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __VARARGS_H
#define __STDARG_H
#define __VARARGS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char *va_list;

#define va_align(x)	(((x) + (sizeof(int) - 1)) & (~(sizeof(int) - 1)))

#define va_dcl		int va_alist;
#define va_start(a)	((void)(a = (char *)(&va_alist)))
#define va_arg(a,t)	((sizeof(t) > sizeof(int)) ? \
	*(t *)(((a) += va_align(sizeof(t))) - va_align(sizeof(t))) : \
	(t)(*(int *)(((a) += sizeof(int)) - sizeof(int))))
#define va_end(a)	((void)((a) = (char *)-1))

#ifdef __cplusplus
	}
#endif

#endif
