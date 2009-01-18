/* This file 'buildoptions.h' contains the UnixLib build options.  */

/* Set to 1 if alloca() should call abort() if no memory can be allocated.
   Otherwise set it to 0.  */
#define __UNIXLIB_ALLOCA_FATAL		1

/* Set to 1 if runtime paranoid checks need to be performed. Otherwise
   set it to 0.  */
#define __UNIXLIB_PARANOID		0

/* Set to 1 if we should check that the entire stack chunk chain is
   valid each time the stack is extended/shrunk. Otherwise set it to 0.  */
#define __UNIXLIB_EXTREMELY_PARANOID	0

/* Set to 1 if we should check that the magic number is valid each
   time the stack is extended/shrunk. Otherwise set it to 0.  */
#define __UNIXLIB_STACK_CHECK_MAGIC	1

/* Set to 1 if DDEUtils' Prefix needs to be set at the beginning of
   each process task. Note this hits bugs in some versions of RISC OS.  */
#define __UNIXLIB_SET_DDEPREFIX		0

/* Set to 1 if we want to support POSIX interval timers.  */
#define __UNIXLIB_FEATURE_ITIMERS	1

/* Set to 1 if we want 'errno' to be per-thread rather than a global
   variable.  It is currently inadvisable to set this option as many
   applications rely on 'errno' being a real variable rather than a
   macro.  GCC's builtin math functions also rely on this.  */
#define __UNIXLIB_ERRNO_THREADED	0

/* Set to 1 if we want to support symlinks.  */
#define __UNIXLIB_SYMLINKS		1
