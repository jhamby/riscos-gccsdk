/* This file 'buildoptions.h' contains the default UnixLib build options.  */

/* Set to 1 if runtime paranoid checks need to be performed. Otherwise
   set it to 0.  */
#ifndef __UNIXLIB_PARANOID
#  define __UNIXLIB_PARANOID		0
#endif

/* Set to 1 if DDEUtils' Prefix needs to be set at the beginning of
   each process task. Note this hits bugs in some versions of RISC OS.  */
#ifndef __UNIXLIB_SET_DDEPREFIX
#  define __UNIXLIB_SET_DDEPREFIX	0
#endif

/* Set to 1 if we want to support POSIX interval timers.  */
#ifndef __UNIXLIB_FEATURE_ITIMERS
#  define __UNIXLIB_FEATURE_ITIMERS	1
#endif

/* Set to 1 if we want 'errno' to be per-thread rather than a global
   variable.  It is currently inadvisable to set this option as many
   applications rely on 'errno' being a real variable rather than a
   macro.  GCC's builtin math functions also rely on this.  */
#define __UNIXLIB_ERRNO_THREADED	0

/* Set to 1 if we want to support symlinks.  */
#ifndef __UNIXLIB_SYMLINKS
#  define __UNIXLIB_SYMLINKS		1
#endif

/* Set to 1 if we want to have chunked stack support (needed for APCS-32,
   not needed for EABI).  */
#ifndef __UNIXLIB_CHUNKED_STACK
#  ifdef __ARM_EABI__
#    define __UNIXLIB_CHUNKED_STACK	0
#  else
#    define __UNIXLIB_CHUNKED_STACK	1
#  endif
#endif

/* Full support for 64-bit LFS.  Note this can not be switched on
   until we have RISC OS support for this.  */
#ifndef __UNIXLIB_LFS64_SUPPORT
#  define __UNIXLIB_LFS64_SUPPORT	0
#endif

/* Following options are only relevant for chunked stack builds.  */
#if __UNIXLIB_CHUNKED_STACK

/* Set to 1 if alloca() should call abort() if no memory can be allocated.
   Otherwise set it to 0.  */
#  ifndef __UNIXLIB_ALLOCA_FATAL
#    define __UNIXLIB_ALLOCA_FATAL		1
#  endif

/* Set to 1 if we should check that the entire stack chunk chain is
   valid each time the stack is extended/shrunk. Otherwise set it to 0.  */
#  ifndef __UNIXLIB_EXTREMELY_PARANOID
#    define __UNIXLIB_EXTREMELY_PARANOID	0
#  endif

#endif /* __UNIXLIB_CHUNKED_STACK */

#if __UNIXLIB_CHUNKED_STACK && defined (___ARM_EABI__)
#  error "EABI based target can not have chunked stacks."
#endif

