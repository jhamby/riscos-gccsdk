/* varargs.h

   Shared C Library support.
   Written by Nick Burrett, 1997 and 1998.  */

#ifndef __VARARGS_H
#define __VARARGS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__LCC__)
/* Definition for the LCC compiler.  */

#ifndef _VA_LIST
#define _VA_LIST
typedef char *__va_list;
#endif

#define va_alist  __builtin_va_alist
#define va_dcl    int __builtin_va_alist;

#define va_start(list) list=(char *) &__builtin_va_alist
#define va_arg(list, mode) \
  (*(mode *)(&(list += \
    ((sizeof(mode)+3U)&~3U))[-(int)((sizeof(mode)+3U)&~3U)]))
#define va_end(list) ((void)0)

/* GCC has its own private copy of varargs.h  */
#elif !defined(__GNUC__)

/* Place a copy of <varargs.h> that is suitable for your compiler
   after this comment.  Then remove the #error line.  */
#error varargs.h has not been implemented for this compiler

#endif

#ifdef __cplusplus
}
#endif

#endif
