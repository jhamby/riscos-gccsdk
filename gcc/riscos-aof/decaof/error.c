/*
 * report errors and warnings
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#include "config.h"
#include <stdio.h>
#include "error.h"

extern char *ourname;		/* in main.c */

void
error(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "%s error: ", ourname);
	vfprintf(stderr, fmt, ap);
	putc('\n', stderr);
	va_end(ap);
}

void
warning(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "%s warning: ", ourname);
	vfprintf(stderr, fmt, ap);
	putc('\n', stderr);
	va_end(ap);
}
