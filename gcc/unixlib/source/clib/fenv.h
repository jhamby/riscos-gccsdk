/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/fenv.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __FENV_H
#define __FENV_H 1

#ifdef __cplusplus
extern "C" {
#endif

/* Define bits representing exceptions in the FPU status word.  */

/* Invalid operation.  */
#define FE_INVALID 1
/* Division by zero.  */
#define FE_DIVBYZERO 2
/* Result not representable due to overflow.  */
#define FE_OVERFLOW 4
/* Result not representable due to underflow.  */
#define FE_UNDERFLOW 8

/* Amount to shift by to convert an exception to a mask bit.  */
#define FE_EXCEPT_SHIFT 16

/* All supported exceptions.  */
#define FE_ALL_EXCEPT \
	(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW)

/* The ARM FPU only supports round-to-nearest.  The other rounding modes
   have to be encoded in the instruction.  */
#define FE_TONEAREST 0

/* Type representing exception flags. */
typedef unsigned long fexcept_t;

/* Type representing floating-point environment.  */
typedef struct
  {
    unsigned long cw;
  }
fenv_t;

/* If the default argument is used we use this value.  */
#define FE_DFL_ENV ((fenv_t *) -1l)
#define _FPU_RESERVED 0xfff0e0f0  /* These bits are reserved.  */
#define _FPU_DEFAULT  0x00000000 /* Default value.  */

/* Type of the control word.  */
typedef unsigned int fpu_control_t;

/* Floating-point exception handling.  */

/* Clear the supported exceptions represented by EXCEPTS.  */
extern void feclearexcept (int __excepts);

/* Store implementation-defined representation of the exception flags
   indicated by EXCEPTS in the object pointed to by FLAGP.  */
extern void fegetexceptflag (fexcept_t *__flagp, int __excepts);

/* Raise the supported exceptions represented by EXCEPTS.  */
extern void feraiseexcept (int __excepts);

/* Set complete status for exceptions indicated by EXCEPTS according to
   the representation in the object pointed to by FLAGP.  */
extern void fesetexceptflag (const fexcept_t *__flagp, int __excepts);

/* Determine which of subset of the exceptions specified by EXCEPTS are
   currently set.  */
extern int fetestexcept (int __excepts);


/* Rounding control.  */

/* Get current rounding direction.  */
extern int fegetround (void);

/* Establish the rounding direction represented by ROUND.  */
extern int fesetround (int __rounding_direction);


/* Floating-point environment.  */

/* Store the current floating-point environment in the object pointed
   to by ENVP.  */
extern void fegetenv (fenv_t *__envp);

/* Save the current environment in the object pointed to by ENVP, clear
   exception flags and install a non-stop mode (if available) for all
   exceptions.  */
extern int feholdexcept (fenv_t *__envp);

/* Establish the floating-point environment represented by the object
   pointed to by ENVP.  */
extern void fesetenv (const fenv_t *__envp);

/* Save current exceptions in temporary storage, install environment
   represented by object pointed to by ENVP and raise exceptions
   according to saved exceptions.  */
extern void feupdateenv (const fenv_t *__envp);

#ifdef __cplusplus
}
#endif

#endif
