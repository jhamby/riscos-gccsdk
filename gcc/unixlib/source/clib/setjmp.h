/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/setjmp.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* ANSI Standard 4.6: Non-Local Jumps <setjmp.h>.  */

#ifndef __SETJMP_H
#define __SETJMP_H

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __GNUC__
#undef  __attribute__
#define __attribute__(x) /* Ignore */
#endif

#ifdef __JMP_BUF_SIZE
#if __JMP_BUF_SIZE < 23
#undef __JMP_BUF_SIZE
#endif
#endif

#ifndef __JMP_BUF_SIZE
#define __JMP_BUF_SIZE 23
#endif

typedef int jmp_buf[__JMP_BUF_SIZE];

/* ANSI says that setjmp is a macro.  */
#ifdef __STDC__
/* -pcc mode doesn't allow circular definitions... */
#define setjmp(jmp_buf) (setjmp(jmp_buf))
#endif

/* Store the current execution state into env.  */
extern int setjmp (jmp_buf env);
/* Jump to the state saved in 'env'. The setjmp call that
   created 'env' will then return 'val'.  If val is zero, then
   setjmp will return 1. */
extern void longjmp (jmp_buf env, int val) __attribute__ ((__noreturn__));

/* POSIX details.  */

/* Calling environment, plus possibly a saved signal mask.  */
typedef struct sigjmp_buf_struct
{
  /* Calling environment.  */
  jmp_buf __jmpbuf;
  /* Saved the signal mask ? */
  int __mask_was_saved;
  __sigset_t saved_mask;
} sigjmp_buf[1];


/* Similar to setjmp. If save sigs is nonzero, the set of
   blocked signals is saved in state and will be restored
   if a siglongjmp is later performed with this state.  */
extern int sigsetjmp (sigjmp_buf state, int savesigs);

/* Jump to the environment saved in env, making the sigsetjmp
   call there return val, or 1 if val is 0.  Restore the signal
   mask if that sigsetjmp call saved it.  */
extern void siglongjmp (const sigjmp_buf env, int val) __attribute__ ((__noreturn__));

/* Internal functions.  */
extern void __sigsetjmp_helper (sigjmp_buf state, int savesigs);
extern void __siglongjmp_helper (const sigjmp_buf state, int savesigs);

#ifdef __cplusplus
	}
#endif

#endif
