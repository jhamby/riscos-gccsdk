/* libmain - flex run-time support library "main" function */

/* $Header: /usr/local/cvsroot/gccsdk/riscos/flex/libmain.c,v 1.1.1.1 2000/07/15 14:51:45 nick Exp $ */

extern int yylex();

int main( argc, argv )
int argc;
char *argv[];
	{
	while ( yylex() != 0 )
		;

	return 0;
	}
