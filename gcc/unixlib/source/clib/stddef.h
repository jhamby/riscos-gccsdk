/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/stddef.h,v $
 * $Date: 2003/04/22 10:59:44 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifndef __STDDEF_H

#if (! defined __need_wchar_t && ! defined __need_size_t \
     && ! defined __need_ptrdiff_t && ! defined __need_NULL \
     && ! defined __need_wint_t)
#define __STDDEF_H

#define __need_wchar_t
#define __need_size_t
#define __need_ptrdiff_t
#define __need_NULL
#define __need_wint_t
#endif


/* Signed type of difference of two pointers.  */
#if !defined __ptrdiff_t_defined && defined __need_ptrdiff_t
#define __ptrdiff_t_defined

#ifndef __PTRDIFF_TYPE__
#define __PTRDIFF_TYPE__ int
#endif
typedef __PTRDIFF_TYPE__ ptrdiff_t;
#endif
#undef __need_ptrdiff_t

/* Unsigned type of sizeof something.  */
#if !defined __size_t_defined && defined __need_size_t
#define __size_t_defined

#ifndef __size_t
#define __size_t 1
#endif

#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned int
#endif

#ifndef size_t
typedef __SIZE_TYPE__ size_t;
#endif

#endif
#undef __need_size_t

/* Wide character type.  */
#if !defined __wchar_t_defined && defined __need_wchar_t
#define __wchar_t_defined

#define _WCHAR_T
#ifndef __WCHAR_TYPE__
#define __WCHAR_TYPE__ int
#endif

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

#endif
#undef __need_wchar_t

#if !defined __wint_t_defined && defined __need_wint_t
#define __wint_t_defined
#define _WINT_T
#ifndef __WINT_TYPE__
#define __WINT_TYPE__ unsigned int
#endif
typedef __WINT_TYPE__ wint_t;
#endif
#undef __need_wint_t

/* Define NULL.  */
#if !defined __NULL_defined && defined __need_NULL
#define __NULL_defined

#ifndef NULL
#define NULL (0)
#endif
#endif
#undef __need_NULL

#ifdef __STDDEF_H

/* Offset of member 'member' in a struct of type 'type'.  */
#define offsetof(type, member) ((size_t) &((type *)0)->member)

#endif /* __STDDEF_H */

#else
#undef __need_wchar_t
#undef __need_size_t
#undef __need_ptrdiff_t
#undef __need_NULL
#undef __need_wint_t
#endif /* ! __STDDEF_H */
