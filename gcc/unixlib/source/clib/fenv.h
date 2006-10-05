/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef _FENV_H
#define _FENV_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#include <bits/fenv.h>
#include <fpu_control.h>

__BEGIN_DECLS

/* Floating-point exception handling.  */

/* Clear the supported exceptions represented by EXCEPTS.  */
extern int feclearexcept (int __excepts) __THROW;

/* Store implementation-defined representation of the exception flags
   indicated by EXCEPTS in the object pointed to by FLAGP.  */
extern void fegetexceptflag (fexcept_t *__flagp, int __excepts) __THROW;

/* Raise the supported exceptions represented by EXCEPTS.  */
extern int feraiseexcept (int __excepts) __THROW;

/* Set complete status for exceptions indicated by EXCEPTS according to
   the representation in the object pointed to by FLAGP.  */
extern int fesetexceptflag (const fexcept_t *__flagp, int __excepts)
     __THROW;

/* Determine which of subset of the exceptions specified by EXCEPTS are
   currently set.  */
extern int fetestexcept (int __excepts) __THROW;


/* Rounding control.  */

/* Get current rounding direction.  */
extern int fegetround (void) __THROW;

/* Establish the rounding direction represented by ROUND.  */
extern int fesetround (int __rounding_direction) __THROW;


/* Floating-point environment.  */

/* Store the current floating-point environment in the object pointed
   to by ENVP.  */
extern int fegetenv (fenv_t *__envp) __THROW;

/* Save the current environment in the object pointed to by ENVP, clear
   exception flags and install a non-stop mode (if available) for all
   exceptions.  */
extern int feholdexcept (fenv_t *__envp) __THROW;

/* Establish the floating-point environment represented by the object
   pointed to by ENVP.  */
extern int fesetenv (const fenv_t *__envp) __THROW;

/* Save current exceptions in temporary storage, install environment
   represented by object pointed to by ENVP and raise exceptions
   according to saved exceptions.  */
extern void feupdateenv (const fenv_t *__envp) __THROW;

__END_DECLS

#endif
