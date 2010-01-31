/* alloca.h header for the RISC OS SharedCLibrary.
   Copyright (c) 2007 UnixLib Developers
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

#ifndef __ALLOCA_H
#define __ALLOCA_H

/* Unsigned type of sizeof something.  */
#ifndef __size_t
#define __size_t 1
#endif
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned int
#endif
#ifndef size_t
typedef __SIZE_TYPE__ size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#undef alloca

/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca (size_t __size);

#ifdef __GNUC__
#  define alloca(size) __builtin_alloca (size)
#endif

#ifdef __cplusplus
}
#endif

#endif
