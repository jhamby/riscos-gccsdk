/* errno.h standard header for the RISC OS SharedCLibrary.
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

#ifndef __ERRNO_H
#define __ERRNO_H

#ifndef __errno
#define __errno errno
#define errno errno
extern volatile int errno;
#endif

/* Domain error. An input argument is outside the domain over
   which the mathematical function is defined.  */
#define EDOM 1

/* Range error. The result of a function cannot be represented
   as a double floating point value.  */
#define ERANGE 2

/* An unrecognised signal is caught by the default signal handler.  */
#define ESIGNUM 3

/* Invalid argument. */
#define EINVAL 4

/* Function not implemented.  */
#define ENOSYS 5

/* System call interrupted.  */
#define EINTR 6

#define __SYS_NERR 7

#endif
