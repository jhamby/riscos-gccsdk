/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/varargs.h,v $
 * $Date: 2000/07/15 14:52:12 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
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
