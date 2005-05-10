/*
 * main function
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 * Copyright 2005 GCCSDK Developers
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "decaof.h"
#include "decode.h"
#include "io.h"
#include "misc.h"
#include "error.h"

char *ourname;			/* program name */
char **files;			/* list of files to decode */
int nfiles;			/* number of files in **files */

short area_contents = 0;	/* print area contents in hex (-a) */
short area_dec = 0;		/* print area declarations (-d) */
short reloc_dir = 0;		/* print relocation directives (-r) */
short debug = 0;		/* print debugging areas (-g) */
short symtab = 0;		/* print symbol table (-s) */
short strtab = 0;		/* print string table (-t) */

static int gotarg = 0;		/* non-zero if some flags where entered */

static void usage (void);

int
main(int argc, char **argv)
{
	ourname = basename(*argv++);
	argc--;

	/*
	 * parse args (can't use getopt() 'cos not all C libraries have it)
	 */
	while(argc) {
		int donext = 0;
		char *arg = *argv;
		if (*arg == '-') {
			char c;
			while (!donext && !isspace(c = *++arg) && c) {
				switch(c) {
				case 'b':
					area_dec++;
					area_contents = 0;
					debug = 0;
					reloc_dir = 0;
					symtab = 0;
					strtab = 0;
					gotarg++;
					break;
				case 'a':
					area_contents++;
					area_dec++;
					gotarg++;
					break;
				case 'd':
					area_dec++;
					gotarg++;
					break;
				case 'r':
					area_dec++;
					reloc_dir++;
					gotarg++;
					break;
				case 'g':
					debug++;
					gotarg++;
					break;
				case 's':
					symtab++;
					gotarg++;
					break;
				case 't':
					strtab++;
					gotarg++;
					break;
				default:
					error("unknown option '%c'", c);
					exit(1);
				}
			}
			argv++;
			argc--;
		} else
			break;
	}

	if (!argc)
		usage();

	files = argv;
	nfiles = argc;

	if (!gotarg) {
		/* set-up default arguments */
		area_dec++;
		symtab++;
		strtab++;
	}

	return(decode());
}


/*
 * display program usage and exit
 */
static void usage(void)
{
	fprintf(stderr, "usage: %s [options] [file ... file]\n",
	    ourname);
	fprintf(stderr, "       where options are:\n");
	fprintf(stderr, "       -b print only the area declarations\n");
	fprintf(stderr, "       -a print area contents in hex\n");
	fprintf(stderr, "       -d print area declarations\n");
	fprintf(stderr, "       -r print relocation directives\n");
	fprintf(stderr, "       -g print debugging areas\n");
	fprintf(stderr, "       -s print symbol table\n");
	fprintf(stderr, "       -t print string table\n");
	exit(1);
}
