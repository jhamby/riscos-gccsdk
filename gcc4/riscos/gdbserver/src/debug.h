#ifndef src_debug_h_
#define src_debug_h_

#ifdef CONFIG_DEBUG
#  ifdef CONFIG_DEBUG_REPORTER
void dprintf (const char *msg, ...)
  __attribute__ ((__format__ (__printf__, 1, 2)));
#  else
#    include <stdio.h>
#    define dprintf printf
#  endif
#else
static inline void
  __attribute__ ((__format__ (__printf__, 1, 2)))
dprintf (const char *msg __attribute__ ((unused)), ...)
{
}
#endif

#endif
