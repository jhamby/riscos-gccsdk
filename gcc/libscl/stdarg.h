/* stdarg.h

   Shared C Library support.
   Written by Nick Burrett, 1997 and 1998.  */

#ifndef __STDARG_H
#define __STDARG_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined(__LCC__)
/* Definition for the LCC compiler.  */

#ifndef _VA_LIST
#define _VA_LIST
typedef char *__gnuc_va_list;
#endif

typedef __gnuc_va_list va_list;
#define va_start(list, start) ((void)((list) = (sizeof(start)<4 ? \
  (char *)((int *)&(start)+1) : (char *)(&(start)+1))))
#define va_arg(list, mode) \
  (*(mode *)(&(list += \
    ((sizeof(mode)+3U)&~3U))[-(int)((sizeof(mode)+3U)&~3U)]))
#define va_end(list) ((void)0)

/* GCC has its own private copy of stdarg.h.  */
#elif !defined(__GNUC__)

/* Place a copy of <stdarg.h> that is suitable for your compiler
   after this comment.  Then remove the #error line.  */

#error stdarg.h has not been implemented for this compiler

#elif defined(__GNUC__)
#error Incorrect stdarg.h included (shouldn't get here)
#endif

#ifdef __cplusplus
}
#endif
#endif
