/* setjmp.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, 2003, Nick Burrett.  */

#ifndef __SETJMP_H
#define __SETJMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Objects of type jmp_buf hold the state information to be
   restored by a non-local exit.  */
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
