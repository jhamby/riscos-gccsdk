/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/unixlib/h/features,v $
 * $Date: 1999/11/16 13:26:49 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* The header file <unixlib/features.h> contains test macros for optionally
   turning on/off certain features that the library provides or integrity
   checks.  */

#ifndef __UNIXLIB_FEATURES_H
#define __UNIXLIB_FEATURES_H 1

/* These macros should be kept in sync with clib.unixlib.s.asm_dec.  */

/* Define this if alloca() should call abort() if no memory can be
   allocated.  */
#define __ALLOCA_FATAL

/* Define if function call references through the dev structure should
   be first checked that their pointer is valid.  */
#undef __PARANOID

/* Define if support for dynamic areas on RISC OS 3.5+ should be compiled
   in.  */
#ifndef __DYNAMIC_AREA
#define __DYNAMIC_AREA 1
#endif

/* Align the stack on a 4K boundary.  */
#undef __4K_BOUNDARY

#undef __USEFILEPATH

/* Define if library functions should check certain parameters for their
   validness.  Undefining will remove these consistency checks.  */
#ifndef __INTEGRITY_CHECK
#define __INTEGRITY_CHECK 1
#endif

/* Define if we want to support POSIX interval timers.  */
#ifndef __FEATURE_ITIMERS
#define __FEATURE_ITIMERS 1
#endif

/* Define if we want Unix socket support.  */
#ifndef __FEATURE_SOCKET
#define __FEATURE_SOCKET 1
#endif

/* Define if we want support for pipes.  */
#ifndef __FEATURE_PIPEDEV
#define __FEATURE_PIPEDEV 1
#endif

/* Define for support of /dev/rs423.  */
#ifndef __FEATURE_DEV_RS423
#define __FEATURE_DEV_RS423 1
#endif

#endif
