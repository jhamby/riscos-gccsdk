/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/sysconf.c,v $
 * $Date: 2000/07/15 14:52:45 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sysconf.c,v 1.1.1.1 2000/07/15 14:52:45 nick Exp $";
#endif

/* unix.c.sysconf. Return system implementation details.

   Written by Nick Burrett, 13 October 1996.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

/* Get the value of the system variable NAME.  */
long int
sysconf (int name)
{
  switch (name)
    {
    default:
      return __set_errno (EINVAL);

    case _SC_ARG_MAX:
#ifdef	ARG_MAX
      return ARG_MAX;
#else
      return -1;
#endif

    case _SC_CHILD_MAX:
#ifdef	CHILD_MAX
      return CHILD_MAX;
#else
      return -1;
#endif

    case _SC_CLK_TCK:
      return CLOCKS_PER_SEC;

    case _SC_NGROUPS_MAX:
#ifdef	NGROUPS_MAX
      return NGROUPS_MAX;
#else
      return -1;
#endif

    case _SC_OPEN_MAX:
      return getdtablesize ();

    case _SC_STREAM_MAX:
      return FOPEN_MAX;

    case _SC_TZNAME_MAX:
#ifdef _POSIX_TZNAME_MAX
      return _POSIX_TZNAME_MAX;
#else
      return -1;
#endif

    case _SC_JOB_CONTROL:
#ifdef	_POSIX_JOB_CONTROL
      return 1;
#else
      return -1;
#endif
    case _SC_SAVED_IDS:
#ifdef	_POSIX_SAVED_IDS
      return 1;
#else
      return -1;
#endif
    case _SC_VERSION:
      return _POSIX_VERSION;

    case _SC_PAGESIZE:
      return getpagesize ();

    case _SC_BC_BASE_MAX:
#ifdef	BC_BASE_MAX
      return BC_BASE_MAX;
#else
      return -1;
#endif

    case _SC_BC_DIM_MAX:
#ifdef	BC_DIM_MAX
      return BC_DIM_MAX;
#else
      return -1;
#endif

    case _SC_BC_SCALE_MAX:
#ifdef	BC_SCALE_MAX
      return BC_SCALE_MAX;
#else
      return -1;
#endif

    case _SC_BC_STRING_MAX:
#ifdef	BC_STRING_MAX
      return BC_STRING_MAX;
#else
      return -1;
#endif

    case _SC_EQUIV_CLASS_MAX:
#ifdef	EQUIV_CLASS_MAX
      return EQUIV_CLASS_MAX;
#else
      return -1;
#endif

    case _SC_EXPR_NEST_MAX:
#ifdef	EXPR_NEST_MAX
      return EXPR_NEST_MAX;
#else
      return -1;
#endif

    case _SC_LINE_MAX:
#ifdef	LINE_MAX
      return LINE_MAX;
#else
      return -1;
#endif

    case _SC_RE_DUP_MAX:
#ifdef	RE_DUP_MAX
      return RE_DUP_MAX;
#else
      return -1;
#endif


    case _SC_2_VERSION:
      /* This is actually supposed to return the version
	 of the 1003.2 utilities on the system {POSIX2_VERSION}.  */
      return _POSIX2_C_VERSION;

    case _SC_2_C_BIND:
#ifdef	_POSIX2_C_BIND
      return _POSIX2_C_BIND;
#else
      return -1;
#endif

    case _SC_2_C_DEV:
#ifdef	_POSIX2_C_DEV
      return _POSIX2_C_DEV;
#else
      return -1;
#endif

    case _SC_2_FORT_DEV:
#ifdef	_POSIX2_FORT_DEV
      return _POSIX2_FORT_DEV;
#else
      return -1;
#endif

    case _SC_2_SW_DEV:
#ifdef	_POSIX2_SW_DEV
      return _POSIX2_SW_DEV;
#else
      return -1;
#endif
    }
}
