/* Return system implementation details.
   Copyright (c) 2001, 2005, 2007, 2008 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include <internal/unix.h>

/* Get the value of the system variable NAME.  */
long int
sysconf (int name)
{
  switch (name)
    {
#ifdef	ARG_MAX
      case _SC_ARG_MAX:
        return ARG_MAX;
#endif

#ifdef	CHILD_MAX
      case _SC_CHILD_MAX:
        return CHILD_MAX;
#endif

      case _SC_CLK_TCK:
        return CLOCKS_PER_SEC;

#ifdef	NGROUPS_MAX
      case _SC_NGROUPS_MAX:
        return NGROUPS_MAX;
#endif

      case _SC_OPEN_MAX:
        return getdtablesize ();

      case _SC_STREAM_MAX:
        return FOPEN_MAX;

#ifdef _POSIX_TZNAME_MAX
      case _SC_TZNAME_MAX:
        return _POSIX_TZNAME_MAX;
#endif

#ifdef	_POSIX_JOB_CONTROL
      case _SC_JOB_CONTROL:
        return 1;
#endif

#ifdef	_POSIX_SAVED_IDS
      case _SC_SAVED_IDS:
        return 1;
#endif

      case _SC_VERSION:
        return _POSIX_VERSION;

      case _SC_PAGESIZE:
        return __ul_global.pagesize;

#ifdef	BC_BASE_MAX
      case _SC_BC_BASE_MAX:
        return BC_BASE_MAX;
#endif

#ifdef	BC_DIM_MAX
      case _SC_BC_DIM_MAX:
        return BC_DIM_MAX;
#endif

#ifdef	BC_SCALE_MAX
      case _SC_BC_SCALE_MAX:
        return BC_SCALE_MAX;
#endif

#ifdef	BC_STRING_MAX
      case _SC_BC_STRING_MAX:
        return BC_STRING_MAX;
#endif

#ifdef	EQUIV_CLASS_MAX
      case _SC_EQUIV_CLASS_MAX:
        return EQUIV_CLASS_MAX;
#endif

#ifdef	EXPR_NEST_MAX
      case _SC_EXPR_NEST_MAX:
        return EXPR_NEST_MAX;
#endif

#ifdef	LINE_MAX
      case _SC_LINE_MAX:
        return LINE_MAX;
#endif

#ifdef	RE_DUP_MAX
      case _SC_RE_DUP_MAX:
        return RE_DUP_MAX;
#endif

      case _SC_2_VERSION:
        /* This is actually supposed to return the version
	   of the 1003.2 utilities on the system {POSIX2_VERSION}.  */
        return _POSIX2_C_VERSION;

#ifdef	_POSIX2_C_BIND
      case _SC_2_C_BIND:
        return _POSIX2_C_BIND;
#endif

#ifdef	_POSIX2_C_DEV
      case _SC_2_C_DEV:
        return _POSIX2_C_DEV;
#endif

#ifdef	_POSIX2_FORT_DEV
      case _SC_2_FORT_DEV:
        return _POSIX2_FORT_DEV;
#endif

#ifdef	_POSIX2_SW_DEV
      case _SC_2_SW_DEV:
        return _POSIX2_SW_DEV;
#endif
    }

  return __set_errno (EINVAL);
}
