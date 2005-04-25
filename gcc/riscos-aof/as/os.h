/* Operating system specific details.

  (c) Copyright Nick Burrett, 1998.
  Part of the assembler AS.  */

#ifndef os_header_included
#define os_header_included

#include "config.h"

#ifdef CROSS_COMPILE
/* UNIX specific information.  */
#define OSCanonicalisePath(a,b,c,d,e) strdup(a)
#else

#include <kernel.h>

extern int switonum (const char *swi);

extern int OSCanonicalisePath (const char *path,
			       char *buffer, int bufferSize,
			       char *systemVar, char *defaultPath);

extern _kernel_oserror *ThrowbackStart (void);
extern _kernel_oserror *ThrowbackSendStart (const char *filename);
extern _kernel_oserror *ThrowbackSendError (int level, long int lineno,
					    const char *error);
extern _kernel_oserror *ThrowbackEnd (void);

#define Throwback_ReasonProcessing	0
#define Throwback_ReasonErrorDetails	1
#define Throwback_ReasonInfoDetails	2

#define ThrowbackInfo			-1
#define ThrowbackWarning		0
#define ThrowbackError			1
#define ThrowbackSeriousError		2

extern char *CanonicalisePath (const char *path);

#endif /* !CROSS_COMPILE */

#ifndef HAVE_STRNDUP
extern char *strndup (const char *str, size_t len);
#endif

#ifndef HAVE_STRDUP
extern char *strdup (const char *str);
#endif

#ifndef HAVE_STRNCASECMP
extern int strncasecmp(const char *str1, const char *str2, size_t n);
#endif

#endif
