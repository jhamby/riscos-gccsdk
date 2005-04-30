/* stdarg.h standard header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

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
