#include <stdio.h>
#include <stdarg.h>

#include "debug.h"

#if defined(CONFIG_DEBUG) && defined(CONFIG_DEBUG_REPORTER)
void
dprintf (const char *msg, ...)
{
  char buf[4096];
  va_list ap;
  
  va_start (ap, msg);
  vsnprintf (buf, sizeof (buf), msg, ap);
  va_end (ap);
  buf[sizeof (buf) - 1] = '\0'; /* Just to be sure.  */
  __asm__ volatile ("MOV\tr0, %[buf]\n\t"
		    "SWI\t%[SWI_XReport_Text0]\n\t"
		    :
		    : [buf] "r" (buf),
                      [SWI_XReport_Text0] "i" (0x54c80 | (1<<17))
		    : "r0", "r14", "cc");
}
#endif
