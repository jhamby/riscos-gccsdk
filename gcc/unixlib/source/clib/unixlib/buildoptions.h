  COMMENT /* This file contains the UnixLib builds options and */
  COMMENT /* gets included by C and ASM files so its syntax is */
  COMMENT /* quite restricted. */

# define __UNIXLIB_ALLOCA_FATAL		1	/* Set to 1 if alloca() should call abort() if no memory can be allocated. Otherwise set it to 0.  */
# define __UNIXLIB_PARANOID		0	/* Set to 1 if runtime paranoid checks need to be performed. Otherwise set it to 0.  */
# define __UNIXLIB_EXTREMELY_PARANOID	0	/* Set to 1 if we should check that the entire stack chunk chain is valid each time the stack is extended/shrunk. Otherwise set it to 0.  */
# define __UNIXLIB_STACK_CHECK_MAGIC	1	/* Set to 1 if we should check that the magic number is valid each time the stack is extended/shrunk. Otherwise set it to 0.  */
# define __UNIXLIB_USEFILEPATH		0	/* */
# define __UNIXLIB_COMPAT_INET4		0	/* Set to 1 if we want Internet 4 compatibility. */

  COMMENT /* Some ARM instructions are very handy and result in */
  COMMENT /* increased performance but they are not supported on */
  COMMENT /* all current ARM architectures. */
# define __UNIXLIB_SWP_SUPPORTED	1	/* Set to 1 if the SWP instruction is supported (not on ARM2). */
# define __UNIXLIB_MULL_SUPPORTED	1	/* Set to 1 if the (U)MULL instructions can be used. */
# define __UNIXLIB_CLZ_SUPPORTED	0	/* Set to 1 if the CLZ instruction can be used. */

# define __UNIXLIB_FEATURE_ITIMERS	1	/* Set to 1 if we want to support POSIX interval timers.  */
# define __UNIXLIB_FEATURE_SOCKET	1	/* Set to 1 if we want Unix socket support.  */
# define __UNIXLIB_FEATURE_PIPEDEV	1	/* Set to 1 if we want support for pipes.  */
# define __UNIXLIB_FEATURE_DEV_RS423	1	/* Set to 1 if we want support of /dev/rs423.  */
# define __UNIXLIB_FEATURE_PTHREADS	1	/* Set to 1 if we want support for pthreads. */

  COMMENT /* The APCS-32 ABI in GCC and Norcroft are not 100% */
  COMMENT /* the same. */
# define __UNIXLIB_NORCROFT_BUILD	0	/* Set to 1 if we want to use the Norcroft ABI */

  END
