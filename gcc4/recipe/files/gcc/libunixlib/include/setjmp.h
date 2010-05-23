/*
 * ANSI Standard 4.6: Non-Local Jumps <setjmp.h>.
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __SETJMP_H
#define __SETJMP_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

#ifdef __TARGET_SCL__
/* SCL needs 22 words, the libscl stubs require one word extra to properly
   unroll alloca() memory blocks.  */
#  define __JMP_BUF_SIZE 23
#else
#  ifdef __SOFTFP__
#   define __JMP_BUF_SIZE 13
#  else
#    define __JMP_BUF_SIZE 25
#  endif
#endif

__BEGIN_NAMESPACE_STD
typedef int jmp_buf[__JMP_BUF_SIZE];

/* Store the current execution state into env.  */
extern int setjmp (jmp_buf __env) __THROW;

/* Jump to the state saved in 'env'. The setjmp call that
   created 'env' will then return 'val'.  If val is zero, then
   setjmp will return 1. */
extern void longjmp (jmp_buf __env, int __val)
     __THROW __attribute__ ((__noreturn__));
__END_NAMESPACE_STD

/* We are in 4.3 BSD-compatibility mode in which `setjmp'
   saves the signal mask like `sigsetjmp (ENV, 1)'.  We have to
   define a macro since ISO C says `setjmp' is one.  */
#define setjmp(env)	setjmp (env)

#ifndef __TARGET_SCL__
#  ifdef __USE_POSIX
/* POSIX details.  */

/* Calling environment, plus possibly a saved signal mask.  */
typedef struct sigjmp_buf_struct
{
  /* Calling environment.  */
  jmp_buf __jmpbuf;
  /* Saved the signal mask ? */
  int __mask_was_saved;
  __sigset_t saved_mask;
  int saved_currently_handling;
} sigjmp_buf[1];

/* Similar to setjmp. If save sigs is nonzero, the set of
   blocked signals is saved in state and will be restored
   if a siglongjmp is later performed with this state.  */
extern int sigsetjmp (sigjmp_buf __state, int __savesigs) __THROW;

/* Jump to the environment saved in env, making the sigsetjmp
   call there return val, or 1 if val is 0.  Restore the signal
   mask if that sigsetjmp call saved it.  */
extern void siglongjmp (sigjmp_buf __env, int __val)
     __THROW __attribute__ ((__noreturn__));
#  endif
#endif

__END_DECLS

#endif
