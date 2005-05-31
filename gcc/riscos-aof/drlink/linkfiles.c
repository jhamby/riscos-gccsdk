/*
** Drlink AOF Linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** This file contains the main procedures for controlling the link
*/

#include "config.h"

#include "drlhdr.h"
#include "filehdr.h"
#include "procdefs.h"

/* #define TIMING */
/* #define DEBUG */

#ifdef TIMING
# include <time.h>
#endif

/*
** 'read_tables' is called to check the various chunks in an
** AOF file and add entries to the various linker tables
*/
bool
read_tables (filelist * fp)
{
  bool ok;

  ok = scan_head (fp) && scan_symt (fp);
  free_srchlist ();

  return ok;
}

/*
** 'link_program' is the main link routine.
**
** One point to note here: 'opt_debug' is set to TRUE if any files
** are in the list of files where debug info is required. The routines
** that handle the debug stuff check 'keepdebug' in the file
** structure to see if it is actually needed
*/
void
link_program (void)
{
#ifdef TIMING
  time_t timer;
#endif
  linkfiles *lp;
  bool ok;

  alloc_filebuffer ();
  create_linksyms ();
  if (opt_verbose)
    error ("Drlink: Checking files...");
#ifdef TIMING
  timer = clock ();
#endif
  link_state = READ_AOF;
  for (lp = linklist, ok = TRUE; lp != NULL && ok; lp = lp->linknext)
    ok = get_files (lp->linkname);
#ifdef TIMING
  printf ("Read files: %d\n", (clock () - timer) / CLOCKS_PER_SEC);
  timer = clock ();
#endif
  ok = ok && resolve ();
  if (ok)
    {
      verify_edits ();
      check_debuglist ();
    }
#ifdef TIMING
  printf ("Resolve references: %d\n", (clock () - timer) / CLOCKS_PER_SEC);
#endif
  opt_debug = opt_debug || debugflist != NULL;	/* Enable 'debug' if necessary */
  if (ok)
    {
      if (imagetype == AOF)
	{			/* Partially-linked AOF file */
	  if (opt_symbols)
	    print_symbols ();
	  create_aofile ();
	  if (opt_areamap)
	    print_areamap ();
	  if (opt_mapfile)
	    print_mapfile ();
	}
      else
	{
	  check_entryarea ();
	  if (opt_nounused)
	    {
#ifdef TIMING
	      timer = clock ();
#endif
	      find_unused ();
#ifdef TIMING
	      printf ("Remove unused areas: %d\n",
		      (clock () - timer) / CLOCKS_PER_SEC);
#endif
	    }

	  link_state = RELOCATE;
	  if (opt_verbose)
	    error ("Drlink: Relocating program...");
#ifdef TIMING
	  timer = clock ();
#endif
	  relocate_areas ();
#ifdef TIMING
	  printf ("Relocate areas: %d\n",
		  (clock () - timer) / CLOCKS_PER_SEC);
#endif
	  relocate_symbols ();
#ifdef TIMING
	  printf ("Relocate symbols: %d\n",
		  (clock () - timer) / CLOCKS_PER_SEC);
	  timer = clock ();
#endif
	  ok = relocate ();
#ifdef TIMING
	  printf ("Relocate program: %d\n",
		  (clock () - timer) / CLOCKS_PER_SEC);
#endif

	  if (ok)
	    {
	      if (opt_cpp)
		build_cdlist ();
	      if (opt_debug)
		ok = build_debugtables ();
	    }
	  if (ok)
	    {
#ifdef TIMING
	      timer = clock ();
#endif
	      create_image ();
#ifdef TIMING
	      printf ("Create image file: %d\n",
		      (clock () - timer) / CLOCKS_PER_SEC);
#endif
	      if (opt_verbose && opt_nounused)
		print_unusedlist ();
	      if (opt_areamap)
		print_areamap ();
	      if (opt_mapfile)
		print_mapfile ();
	      if (opt_symbols)
		print_symbols ();
	    }
	}
    }
#ifdef DEBUG
  if (opt_dump)
    {
      list_allareas ();
      list_files ();
    }
#endif

#ifndef CROSS_COMPILE
  if (opt_throw)
    end_throwback ();
#endif
}
