/* setjmp.h standard header for the RISC OS SharedCLibrary.
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

#ifndef __SETJMP_H
#define __SETJMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Objects of type jmp_buf hold the state information to be
   restored by a non-local exit.  */
/* SCL needs 22 words, the libscl stubs require one word extra to properly
   unroll alloca() memory blocks.  */
#undef __JMP_BUF_SIZE
#define __JMP_BUF_SIZE 23
#ifdef __JMP_BUF_SIZE
typedef int jmp_buf[__JMP_BUF_SIZE];
#else
typedef int jmp_buf[22];
#endif

/* setjmp stores information about the execution state of the
   program in 'state' and returns zero.  If longjmp is later
   used to perform a non-local exit to this 'state', setjmp
   returns a nonzero value.  */
extern int setjmp (jmp_buf __state);

#ifdef __STDC__
/* ANSI states that setjmp must be a macro.  */
#define setjmp(jmp_buf) (setjmp(jmp_buf))
#endif

/* Restore the current execution to the state saved in 'state'.
   Returning from setjmp by means of longjmp returns 'value'
   argument that was passed to longjmp rather than 0.  If 'value'
   is 0, setjmp returns 1.  */
extern void longjmp (jmp_buf __state, int __value) __attribute__ ((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif
