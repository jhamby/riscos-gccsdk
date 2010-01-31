/* assert.h standard header for the RISC OS SharedCLibrary.
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

/* Allow this file to be included multiple times
   with different settings of NDEBUG.  */
#undef assert
#undef __assert

#ifdef NDEBUG
#define assert(ignore) ((void) 0)
#else

#ifndef __GNUC__

#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (printf ("%s:%u: failed assertion\n", file, lineno),	\
   abort (), 0)

#else

#if defined(__STDC__) || defined (__cplusplus)

#ifdef __cplusplus
extern "C" void __assert (const char *, const char *, int);
extern "C" int __assert2(const char *, const char *, const char *, int);
#else
extern void __assert(const char *, const char *, int);
extern void __assert2(const char *, const char *, const char *, int);
#endif


#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert2 (#expression, __func__, __FILE__, __LINE__)))

#else /* no __STDC__ and not C++; i.e. -traditional.  */

extern void __assert(const char *, const char *, int);

#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (#expression, __FILE__, __LINE__)))

#endif /* no __STDC__ and not C++; i.e. -traditional. */
#endif /* no __GNU__; i.e., /bin/cc.  */
#endif
