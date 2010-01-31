/* Functions for internally debugging UnixLib.
   Copyright (c) 2004, 2005, 2008 UnixLib Developers.  */

#ifdef DEBUG

#ifndef __SYS_DEBUG_H
#define __SYS_DEBUG_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

/* __debug(s) dumps UNIX status with title __s.  __s may be NULL when there
   is no title to be printed.  */
extern void __debug(const char *__s);

/* Works like the standard printf function but outputs using standard
   RISC OS SWIs to the console.  */
extern void debug_printf (const char *__fmt, ...)
     __attribute__ ((__format__ (__printf__, 1, 2)));

__END_DECLS

#endif

#endif
