/* stdarg.h
   Shared C Library support.
   Written by Nick Burrett, 1997 and 1998, 2003.  */

#ifdef __GNUC__
/* If we're GCC, then we really don't want to be using this file.
   Luckily this only occurs when building the SDK.  */
#include_next <stdarg.h>

#else /* ! __GNUC__ */

#ifndef __STDARG_H
#ifndef __need___va_list
#define __STDARG_H
#endif
#undef __need___va_list

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef char * __gnuc_va_list;
#endif

#ifdef __STDARG_H

#if defined(__LCC__)
/* Definition for the LCC compiler.  */

typedef __gnuc_va_list va_list;
#define va_start(list, start) ((void)((list) = (sizeof(start)<4 ? \
  (char *)((int *)&(start)+1) : (char *)(&(start)+1))))
#define va_arg(list, mode) \
  (*(mode *)(&(list += \
    ((sizeof(mode)+3U)&~3U))[-(int)((sizeof(mode)+3U)&~3U)]))
#define va_end(list) ((void)0)

#else
/* Definitions for the Norcroft compiler.  */

#define va_align(x)     (((x) + (sizeof(int) - 1)) & (~(sizeof(int) - 1)))

#define va_start(a,p)   ((void)((a) = ((char *)(&(p)) + va_align(sizeof(p)))))
#define va_arg(a,t)     ((sizeof(t) > sizeof(int)) ? \
        *(t *)(void *)(((a) += va_align(sizeof(t))) - va_align(sizeof(t))) : \
        (t)(*(int *)(void *)(((a) += sizeof(int)) - sizeof(int))))
#define va_end(a)       ((void)((a) = (char *)-1))
#define va_copy(dest, src) (dest) = (src)

#endif

#endif /* __STDARG_H */

#ifndef _VA_LIST_
typedef __gnuc_va_list va_list;
#define _VA_LIST_
#endif /* __va_list__ */
#endif /* _VA_LIST_ */

#endif /* __GNUC__ */
