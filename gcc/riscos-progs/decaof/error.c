/*
 * report errors and warnings
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#include <stdio.h>
#include "error.h"

extern char *ourname;		/* in main.c */

void
#ifdef ANSI_C
error(char *fmt, ...)
#else
error(va_alist)
va_dcl
#endif /* ANSI_C */
{
	va_list ap;
#ifndef ANSI_C
	char *fmt;
	va_start(ap);
	fmt = va_arg(ap, char *);
#else
	va_start(ap, fmt);
#endif /* !ANSI_C */
	fprintf(stderr, "%s error: ", ourname);
#ifdef BSD42
	_doprnt(fmt,ap,stderr);
#else
	vfprintf(stderr, fmt, ap);
#endif /* BSD42 */
	putc('\n', stderr);
	va_end(ap);

}

void
#ifdef ANSI_C
warning(char *fmt, ...)
#else
warning(va_alist)
va_dcl
#endif /* ANSI_C */
{
	va_list ap;
#ifndef ANSI_C
	char *fmt;
	va_start(ap);
	fmt = va_arg(ap, char *);
#else
	va_start(ap, fmt);
#endif /* !ANSI_C */
	fprintf(stderr, "%s warning: ", ourname);
#ifdef BSD42
	_doprnt(fmt,ap,stderr);
#else
	vfprintf(stderr, fmt, ap);
#endif /* BSD42 */
	putc('\n', stderr);
	va_end(ap);
}
