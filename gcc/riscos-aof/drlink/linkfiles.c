/*
** Drlink AOF Linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This file contains the main procedures for controlling the link
*/

#include "drlhdr.h"
#include "filehdr.h"
#include "procdefs.h"

/* #define TIMING */
/* #define DEBUG */

#ifdef TIMING
#include <time.h>
#endif

/*
** 'read_tables' is called to check the various chunks in an
** AOF file and add entries to the various linker tables
*/
bool read_tables(filelist *fp) {
  bool ok;
  ok = scan_head(fp);
  ok = ok && scan_symt(fp);
  free_srchlist();
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

#ifdef TIMING

void link_program(void) {
  time_t timer;
  linkfiles *lp;
  bool ok;
  alloc_filebuffer();
  create_linksyms();
  if (opt_verbose) error("Drlink: Checking files...");
  timer = clock();
  link_state = READ_AOF;
  lp = linklist;
  ok = TRUE;
  do {
    ok = get_files(lp->linkname);
    lp = lp->linknext;
  } while (lp!=NIL && ok);
  if (ok) verify_edits();
  printf("Read files: %d\n", (clock()-timer) / CLOCKS_PER_SEC);
  timer = clock();
  ok = ok && resolve();
  if (ok) check_debuglist();
  printf("Resolve references: %d\n", (clock()-timer) / CLOCKS_PER_SEC);
  opt_debug = opt_debug || debugflist!=NIL;	/* Enable 'debug' if necessary */
  if (ok) {
    if (imagetype==AOF) {	/* Partially-linked AOF file */
      if (opt_symbols) print_symbols();
      create_aofile();
      if (opt_areamap) print_areamap();
      if (opt_mapfile) print_mapfile();
    }
    else {
      check_entryarea();
      timer = clock();
      if (opt_nounused) find_unused();
      printf("Remove unused areas: %d\n", (clock()-timer) / CLOCKS_PER_SEC);
      timer = clock();
      link_state = RELOCATE;
      if (opt_verbose) error("Drlink: Relocating program...");
      relocate_areas();
      relocate_symbols();
      printf("Relocate symbols: %d\n", (clock()-timer) / CLOCKS_PER_SEC);
      timer = clock();
      ok = relocate();
      printf("Relocate program: %d\n", (clock()-timer) / CLOCKS_PER_SEC);
      if (ok) {
        if (opt_cpp) build_cdlist();
        if (opt_debug) ok = build_debugtables();
      }
      if (ok) {
        timer = clock();
        create_image();
        printf("Create image file: %d\n", (clock()-timer) / CLOCKS_PER_SEC);
        if (opt_verbose && opt_nounused) print_unusedlist();
        if (opt_areamap) print_areamap();
        if (opt_mapfile) print_mapfile();
        if (opt_symbols) print_symbols();
      }
    }
  }
#ifdef DEBUG
  if (opt_dump) {
    list_allareas();
    list_files();
  }
#endif

#ifdef TARGET_RISCOS
  if (opt_throw) end_throwback();
#endif
}

#else

/* This version leaves out all the timing code */

void link_program(void) {
  linkfiles *lp;
  bool ok;
  alloc_filebuffer();
  create_linksyms();
  if (opt_verbose) error("Drlink: Checking files...");
  link_state = READ_AOF;
  lp = linklist;
  ok = TRUE;
  do {
    ok = get_files(lp->linkname);
    lp = lp->linknext;
  } while (lp!=NIL && ok);
  if (ok) verify_edits();
  ok = ok && resolve();
  if (ok) check_debuglist();
  opt_debug = opt_debug || debugflist!=NIL; /* Enable 'debug' if necessary */
  if (ok) {
    if (imagetype==AOF) {	/* Partially-linked AOF file */
      if (opt_symbols) print_symbols();
      create_aofile();
      if (opt_areamap) print_areamap();
      if (opt_mapfile) print_mapfile();
    }
    else {
      check_entryarea();
      if (opt_nounused) find_unused();
      link_state = RELOCATE;
      if (opt_verbose) error("Drlink: Relocating program...");
      relocate_areas();
      relocate_symbols();
      ok = relocate();
      if (ok) {
        if (opt_cpp) build_cdlist();
        if (opt_debug) ok = build_debugtables();
      }
      if (ok) {
        create_image();
        if (opt_verbose && opt_nounused) print_unusedlist();
        if (opt_areamap) print_areamap();
        if (opt_mapfile) print_mapfile();
        if (opt_symbols) print_symbols();
      }
    }
  }
#ifdef DEBUG
  if (opt_dump) {
    list_allareas();
    list_files();
  }
#endif

#ifdef TARGET_RISCOS
  if (opt_throw) end_throwback();
#endif
}

#endif
