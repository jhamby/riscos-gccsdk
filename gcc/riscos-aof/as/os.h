/* Operating system specific details.

  (c) Copyright Nick Burrett, 1998.
  Part of the assembler AS.  */

#ifndef os_header_included
#define os_header_included

#include <stdio.h>
#include "config.h"

#ifdef CROSS_COMPILE
/* UNIX specific information.  */
#define OSCanonicalisePath(a,b,c,d,e) strdup(a)
#else

/* Acorn/RISC OS specific information.  */
typedef struct os_error
{
  int errnum;
  char errmess[256];
}
os_error;

extern char *ErrorFile;

int switonum (const char *swi);


int (OSCanonicalisePath) (char *path,
			  char *buffer, int bufferSize,
			  char *systemVar, char *defaultPath);

os_error *ThrowbackStart (void);
os_error *ThrowbackSendStart (const char *filename);
os_error *ThrowbackSendError (int level, long int lineno, const char *error);
os_error *ThrowbackEnd (void);


#define Throwback_ReasonProcessing	0
#define Throwback_ReasonErrorDetails	1
#define Throwback_ReasonInfoDetails	2

#define ThrowbackInfo			-1
#define ThrowbackWarning		0
#define ThrowbackError			1
#define ThrowbackSeriousError		2

char *(CanonicalisePath) (const char *path);

#endif /* !CROSS_COMPILE */

#if defined CROSS_COMPILE && !defined (HAVE_STRNDUP)
extern char *strndup (const char *str, size_t len);
#endif
#if defined CROSS_COMPILE && !defined (HAVE_STRDUP)
extern char *strdup (const char *str);
#endif
#if defined CROSS_COMPILE && !defined (HAVE_STRNCASECMP)
extern int strncasecmp(const char *str1, const char *str2, size_t n);
#endif

#endif
