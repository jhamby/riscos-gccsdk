/*
 * main function
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2012 GCCSDK Developers
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
main (int argc, char **argv)
{
  argv++;
  argc--;

  /*
   * parse args (can't use getopt() 'cos not all C libraries have it)
   */
  while (argc)
    {
      int donext = 0;
      char *arg = *argv;
      if (*arg == '-')
	{
	  char c;
	  while (!donext && !isspace (c = *++arg) && c)
	    {
	      gotarg++;
	      switch (c)
		{
		  case 'h':
		    usage ();
		    break;
		  case 'b': /* brief */
		  case 'd': /* full */
		    area_dec++;
		    break;
		  case 'a':
		    area_contents++;
		    area_dec++;
		    break;
		  case 'r':
		    reloc_dir++;
		    area_dec++;
		    break;
		  case 'g':
		    debug++;
		    break;
		  case 's':
		    symtab++;
		    break;
		  case 't':
		    strtab++;
		    break;
		  default:
		    error ("unknown option '%c'", c);
		    exit (EXIT_FAILURE);
		}
	    }
	  argv++;
	  argc--;
	}
      else
	break;
    }

  if (!argc)
    usage ();

  files = (const char **) argv;
  nfiles = argc;

  if (!gotarg)
    {
      /* Set-up default arguments : full option. */
      area_contents++;
      area_dec++;
      reloc_dir++;
      debug++;
      symtab++;
      strtab++;
    }

  decode ();
  return (nrerrs) ? EXIT_FAILURE : EXIT_SUCCESS;
}


/*
 * display program usage and exit
 */
static void
usage (void)
{
  fprintf (stderr,
	   "GCCSDK Decode AOF files " VERSION " (r" GCCSDK_REVISION ", " __DATE__ ")\n"
	   "\n"
	   "Usage: " PACKAGE " [options] file [file...]\n"
	   "\n"
	   "where options are:\n"
	   "       -b print only the area declarations\n"
	   "       -a print area contents in hex\n"
	   "       -d print area declarations\n"
	   "       -r print relocation directives\n"
	   "       -g print debugging areas\n"
	   "       -s print symbol table\n"
	   "       -t print string table\n");
  exit (EXIT_FAILURE);
}
