/*
 * main function
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "decaof.h"
#include "decode.h"
#include "error.h"
#include "io.h"
#include "misc.h"
#include "main.h"

const char *ourname;		/* program name */
const char **files;		/* list of files to decode */
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
					exit(EXIT_FAILURE);
				}
			}
			argv++;
			argc--;
		} else
			break;
	}

	if (!argc)
		usage();

	files = (const char **)argv;
	nfiles = argc;

	if (!gotarg) {
		/* set-up default arguments */
		area_dec++;
		symtab++;
		strtab++;
	}

	decode();
	return (nrerrs) ? EXIT_FAILURE : EXIT_SUCCESS;
}


/*
 * display program usage and exit
 */
static void usage(void)
{
	fprintf(stderr, "Decode AOF files v2.00 (" __DATE__ ") [GCCSDK " __VERSION__ "]\n\n"
			"Usage: %s [options] file [file...]\n"
			"       where options are:\n"
			"       -b print only the area declarations\n"
			"       -a print area contents in hex\n"
			"       -d print area declarations\n"
			"       -r print relocation directives\n"
			"       -g print debugging areas\n"
			"       -s print symbol table\n"
			"       -t print string table\n", ourname);
	exit(EXIT_FAILURE);
}
