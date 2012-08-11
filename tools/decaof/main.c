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

bool opt_print_area_contents = false;	/* print area contents in hex (-a) */
bool opt_print_area_dec = false;	/* print area declarations (-d) */
bool opt_print_reloc_dir = false;	/* print relocation directives (-r) */
bool opt_print_debug = false;		/* print debugging areas (-g) */
bool opt_print_symtab = false;		/* print symbol table (-s) */
bool opt_print_strtab = false;		/* print string table (-t) */
bool opt_print_ident = false;		/* print identification (no user option) */

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
		    opt_print_area_dec = true;
		    break;
		  case 'a':
		    opt_print_area_contents = true;
		    opt_print_area_dec = true;
		    break;
		  case 'r':
		    opt_print_reloc_dir = true;
		    opt_print_area_dec = true;
		    break;
		  case 'g':
		    opt_print_debug = true;
		    break;
		  case 's':
		    opt_print_symtab = true;
		    break;
		  case 't':
		    opt_print_strtab = true;
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
      opt_print_area_contents = true;
      opt_print_area_dec = true;
      opt_print_reloc_dir = true;
      opt_print_debug = true;
      opt_print_symtab = true;
      opt_print_strtab = true;
      opt_print_ident = true;
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
