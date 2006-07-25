/*
** Drlink AOF linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006  GCCSDK Developers
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
** This file contains constants and flags used throughout the linker
*/

#ifndef drlhdr_h
#define drlhdr_h

/* First, say which environment the program is being compiled to run under */

#ifndef CROSS_COMPILE
# define IGNORE_CASE
#endif

#define DL_VERSION "v0.44 (" __DATE__ ") [GCCSDK " __VERSION__ "]"

/*
** Some constants used everywhere
*/

#define FALSE 0
#define TRUE 1
#define COERCE(x,t) ((t)(void *)(x))

#define FNAMELEN 500		/* Maximum filename length */
#define LEAFLEN 100		/* Maximum leaf name length */
#define MSGBUFLEN 2000		/* General message buffer length */
#define MAXCOMMON 32		/* Number of common block symbol lists (must be a power of two) */
#define MAXLOCALS 32		/* Number of local symbol lists (must be a power of two) */
#define MAXGLOBALS 256		/* Number of global symbol lists (must be a power of two) */
#define MAXENTRIES 128		/* Number of library lists */
#define COMMONMASK (MAXCOMMON-1)
#define LOCALMASK (MAXLOCALS-1)
#define GLOBALMASK (MAXGLOBALS-1)
#define LIBEMASK (MAXENTRIES-1)

#define ALIGNSIZE (sizeof(int))
#define ALIGNMASK (-ALIGNSIZE)

typedef enum {	/* Linker states */
  STARTING,
  READ_AOF,
  AOF_SEARCH,
  LIB_SEARCH,
  RELOCATE,
  CREATE_IMAGE
} linker_state;

typedef char bool;

/*
** Variables found in 'drlmain.c' used in other files
*/
extern int warnings,		/* Number of warning messages put out by linker */
  errors;			/* Number of error messages put out by linker */

extern bool inviafile,		/* TRUE if taking commands from 'via' file */
  low_memory,			/* TRUE if memory is running low */
  aofv3flag,			/* TRUE if any AOF files use AOF version 3 */
  got_oldlibs,			/* TRUE if an old-style library has been read */
  got_26bitapcs,		/* TRUE if any code area complies to a 26 bit APCS */
  got_32bitapcs,		/* TRUE if any code area complies to a 32 bit APCS */
  opt_verbose,			/* TRUE if linker is putting out progress messages */
  opt_quiet,			/* TRUE if linker is not printing non-error messages */
  opt_info,			/* TRUE if printing summary at end of link */
  opt_throw,			/* TRUE if using throwback */
  opt_nounused,			/* TRUE if leaving out unreferenced areas */
  opt_areamap,			/* TRUE if linker will produce an area map */
  opt_mapfile,			/* TRUE if linker will produce an area map file */
  opt_symbols,			/* TRUE if linker will produce a symbol list */
  opt_acornmap,			/* TRUE if generating symbol listing in Acorn format */
  opt_revmap,			/* TRUE if listing symbols <addr> <name> */
  opt_rescan,			/* TRUE if scanning libraries more than once */
  opt_debug,			/* TRUE if linker will include all debug tables */
  opt_keepdebug,		/* TRUE if keeping only some debug tables */
  opt_leaveweak,		/* TRUE if not resolving weak externals using libraries */
  opt_debimage,			/* TRUE if creating 'DebImage' file instead of 'Absolute' */
  opt_codebase,			/* TRUE if a non-standard value the code base address is given */
  opt_database,			/* TRUE if a non-standard value for the R/W data base address is given */
#ifdef DEBUG
  opt_dump,			/* TRUE if a dump of linker structures is needed */
#endif
  opt_case,			/* TRUE if linker will ignore symbol case */
  opt_cpp,			/* TRUE if linking a C++ program */
  opt_linkersets,               /* TRUE if linking in linker-sets */
  opt_gccareas,			/* TRUE if not touching GCC-specific areas in 'nounused' processing */
  opt_pagealign,		/* TRUE if aligning start of R/W areas on page boundary */
  opt_strongarm,		/* TRUE if handling StrongARM-specific stuff */
  opt_warningerrors;		/* TRUE if warnings are treated as errors */           

extern linker_state link_state;	/* Says what the linker is doing */

#define EXIT_OK 0
#define EXIT_ERROR 12
#define EXIT_FATAL 16

#endif
