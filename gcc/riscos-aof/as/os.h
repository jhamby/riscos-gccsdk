/* Operating system specific details.

  (c) Copyright Nick Burrett, 1998.
  Part of the assembler AS.  */

#ifndef os_header_included
#define os_header_included

#include <stdio.h>

#include "sdk-config.h"

#ifdef CROSS_COMPILE
/* UNIX specific information.  */
#define OSCanonicalisePath(a,b,c,d,e) strdup(a)
#define CanonicalisePath(x) (x)
#define toriscos(f,a,b)     (f)

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
os_error *cdir (const char *name);


int (OSCanonicalisePath) (char *path,
			  char *buffer, int bufferSize,
			  char *systemVar, char *defaultPath);
int OSArgs7 (const FILE * fh, char *buffer, int bufferSize);

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

char *(toriscos) (char *name, char *oldsuffixes, char newsuffix);
char *(CanonicalisePath) (const char *path);

#endif /* !CROSS_COMPILE */

char *CanonicaliseFile (const FILE * path);

#if defined CROSS_COMPILE && !defined (HAVE_STRNDUP)
char *strndup (const char *str, size_t len);
#endif
#if defined CROSS_COMPILE && !defined (HAVE_STRDUP)
char *strdup (const char *str);
#endif
#if defined CROSS_COMPILE && !defined (HAVE_STRNCASECMP)
int strncasecmp(const char *str1, const char *str2, size_t n);
#endif

#endif
