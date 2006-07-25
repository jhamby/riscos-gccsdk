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
** This is the main module of the linker. It deals with the
** processing of the command line and any options
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "drlhdr.h"
#include "filehdr.h"
#include "libraries.h"
#include "procdefs.h"

#ifndef CROSS_COMPILE
# include <kernel.h>
#endif

/* Variables referenced by other modules */

int warnings,			/* Number of warning messages put out by linker */
  errors;			/* Number of error messages put out by linker */

bool inviafile,			/* TRUE if taking commands from 'via' file */
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
  opt_gccareas,			/* TRUE if not touching special GCC areas in 'nounused' processing */
  opt_pagealign,		/* TRUE if aligning start of R/W areas on page boundary */
  opt_strongarm,		/* TRUE if handling StrongARM-specific stuff */
  opt_warningerrors;		/* TRUE if warnings are treated as errors */           

linker_state link_state;	/* Says what the linker is doing */

/* Private declarations */

static char *cmdbuffer,		/* Pointer to buffer holding command line */
 *cmdbufend;			/* Pointer to end of command line buffer */

/* Forward references */

static bool startup (void);
static bool copy_cmdline (int, char *[]);
static bool scan_cmdline (void);

/*
** This is where it all begins...
*/
int
main (int argc, char *argv[])
{
  time_t startime;

  errors = 0;
  if (startup ())
    {
      startime = clock ();
      copy_cmdline (argc, argv);
      if (scan_cmdline ())
	{
	  if (!opt_quiet)
	    announce ();
	  link_program ();
	}
      if (opt_verbose || opt_info)
	{
	  int elapsed = (clock () - startime) / (CLOCKS_PER_SEC / 100);
	  printf
	    ("Drlink: Link %s with %d warning%s and %d error%s in %d.%02d seconds\n",
	     (errors == 0 ? "completed" : "failed"), warnings,
	     (warnings == 1 ? "" : "s"), errors, (errors == 1 ? "" : "s"),
	     elapsed / 100, elapsed % 100);
	  print_heapstats ();
	}
      else if (errors > 0)
	{
	  printf ("Drlink: Link failed with %d error%s\n", errors,
		  (errors == 1 ? "" : "s"));
	}
      release_heap ();
    }
  return (errors == 0) ? EXIT_OK : EXIT_ERROR;
}


static void
print_help (void)
{
  announce ();
  printf ("\n'Drlink' is a linker for object files in Acorn's AOF format. It\n"
         "carries out the same function as the Acorn linker, 'link', but it\n"
         "is not a replacement for 'link' as it omits some of that program's\n"
         "features such as the ability to create image files with overlays.\n"
         "\nThe command syntax is:\n\n"
         "  drlink <options> <list of files to include in linked program>\n\n"
         "Options recognised are:\n\n"
         "  -aco[rnmap]  Produce symbol listing in format similar to that of 'link'\n"
         "  -ai[f]       Produce an AIF image file (default)\n"
         "  -ao[f]       Produce a partially-linked AOF file\n"
         "  -area[map] <file> Write list of areas in image file to <file>\n"
         "  -b[ase] <address> Start read only part of image at <address>\n"
         "  -bi[n]       Produce a plain binary image file\n"
         "  -c++         Carry out extra linker actions needed by C++ programs\n"
         "  -c[ase]      Ignore the case of symbols where searching for them\n"
         "  -da[ta] <address> Start read/write part of image at <address>\n"
#ifndef CROSS_COMPILE
         "  -debi[mage]  Set filetype of image file with debug info to 'DebImage'\n"
#endif
#ifdef DEBUG
         "  -dump        Dumps the internal state after linking\n"
#endif
         "  -d[ebug]     Include debugging information in the image file\n"
         "  -e[dit] <file> Take link edit commands from <file>\n"
         "  -gcc         Do not remove GCC-specific areas when '-nounused' used\n"
         "  -h[elp]      Print this help information\n"
         "  -keep[debug] <list> Keep debug info for files in <list> only\n"
         "  -leave[weak] Do not attempt to resolve weak externals using libraries\n"
         "  -lib <list>  Do not load libraries in <list> into memory\n"
         "  -linkersets  Carry out linker-set linkages\n"
         "  -map         Print a list of areas in the image file\n"
         "  -m[odule]    Create a relocatable module\n"
         "  -no[unused]  Leave unreferenced areas out of the image file\n"
         "  -o[utput] <file> Write executable image to file <file>\n"
         "  -pag[ealign] Align end of read-only areas on page boundary\n"
         "  -qui[et]     Do not print any messages except error messages\n"
         "  -r[elocatable] Create a relocatable AIF image file\n"
         "  -res[can]    Scan libraries more than once to find symbols\n"
         "  -rev[map]    Produce symbol listing in order <addr> <symbol>\n"
         "  -rm[f]       Create a relocatable module\n"
         "  -s[ymbols] <file> Write list of symbols in image to <file>\n"
         "  -str[ong]    Warn for StrongARM specific behaviour\n"
         "  -strict      Warnings are treated as errors\n"
#ifndef CROSS_COMPILE
         "  -t[hrowback] Send warning and error messages to throwback window\n"
#endif
         "  -v[erbose]   Print messages as the link progresses\n"
         "  -via <file>  Continue taking link parameters from file <file>\n"
         "  -w[orkspace] <size> Set relocatable image workspace to <size> bytes\n"
         "  -x[ref]      Print a list of references between areas (ignored)\n\n"
         "Note that the following 'link' options, although recognised, have\n"
         "not been implemented and will be flagged as an error: dbug, entry\n"
         "and overlay.\n");
}

/*
** 'get_text' returns a pointer to the next 'word' in the command
** line or 'via' file, adding a null to the end of the word and
** converting it to lower case if it starts with a '-'. It returns
** 'NULL' if at the end of the command line buffer
*/
static char *
get_text (void)
{
  char terminator;
  char *cp, *start;
  bool option;
  cp = cvptr;
  while (cp != cvptrend && *cp <= ' ' && *cp != '\0')
    cp++;
  if (cp == cvptrend || *cp == '\0')
    {				/* At end of cmdline or 'via' file */
      if (inviafile)
	{			/* End of 'via' file */
	  cp = cvptr = lastcvptr;	/* Go back to command line */
	  cvptrend = cmdbufend;
	  inviafile = FALSE;
	  return get_text ();
	}
      return NULL;
    }
  option = *cp == '-';
  if (*cp == '"')
    {				/* Something in quotes */
      terminator = '"';
      cp++;
    }
  else
    {
      terminator = ' ';
    }
  start = cp;
  do
    {
      if (option)
	*cp = tolower (*cp);
      cp++;
    }
  while (cp != cvptrend
	 && ((terminator == '"' && *cp != '"')
	     || (terminator == ' ' && *cp > ' ')));
  if (terminator == '"' && cp == cvptrend)
    {				/* " missing */
      error ("Error: Unmatched '\"' found");
      cvptr = cp;
      return NULL;
    }
  *cp = '\0';
  if (cp != cvptrend)
    cp++;			/* Skip 'null' just added */
  cvptr = cp;
  return start;
}

/*
** 'addto_linklist' adds a file to the list of files to be linked
** into the executable image
*/
static void
addto_linklist (const char *p)
{
  linkfiles *lp;
  if ((lp = allocmem (sizeof (linkfiles))) == NULL)
    error ("Fatal: Out of memory in 'addto_linklist'");
  lp->linkname = p;
  lp->linknext = NULL;
  if (linklist == NULL)
    linklist = lp;
  else
    linklast->linknext = lp;
  linklast = lp;
}

static char *
skip_blanks (char *p)
{
  while (isspace (*p))
    p++;
  return p;
}

char *fnstart, *fnend;		/* Start and end of a filename */

/*
** 'extract_name' is called to find the next name in the string
** pointed at by 'np'. It returns 'TRUE' if a name was found
** otherwise it returns 'FALSE'. 'fnstart' points at the start
** of the string. If this is NULL, the end of the string has been
** reached. The routine adds a null after a name to mark its end.
** Names can be separated by blanks or commas
*/
static bool
extract_name (char *np)
{
  char ch;
  np = skip_blanks (np);
  fnstart = np;
  ch = *np;
  if (ch == '\0')
    {				/* End of string */
      fnstart = NULL;
      return TRUE;
    }
  while (ch != '\0' && ch != ',' && ch != ' ')
    {
      np++;
      ch = *np;
    }
  if (np == fnstart)
    return FALSE;		/* No text found */
  if (ch == ',' || ch == ' ')
    {				/* Another name to follow */
      *np = '\0';
      np++;
    }
  fnend = np;
  return TRUE;
}

/*
** 'get_libraries' is called to verify that the list of libraries
** passed to it exist and adds them to the library list. Note that
** any 'old style' libraries or AOF files found in the list are
** added to the list of files to be loaded in the normal way.
*/
static bool
get_libraries (char *lp)
{
  do
    {
      if (!extract_name (lp))
	{
	  error
	    ("Error: Badly formed library list found after option '-lib'");
	  return FALSE;
	}

      if (fnstart != NULL)
	{
	  switch (examine_file (fnstart))
	    {
	      case NOWT:
	        return FALSE;

	      case LIBRARY:
	        if (!addto_liblist (fnstart, NULL, find_filesize (fnstart)))
	          return FALSE;
	        break;

	      case AOFILE:
	      case OLDLIB:
	        addto_linklist (fnstart);
	        break;
	    }
	}
      lp = fnend;
    }
  while (fnstart != NULL);
  return TRUE;
}

/*
** 'get_debugfiles' adds the files listed to the list of files
** where debug information is to be kept.
*/
static bool
get_debugfiles (char *lp)
{
  bool ok;
  do
    {
      ok = extract_name (lp);
      if (!ok)
	{
	  error
	    ("Error: Badly formed file list found after option '-keepdebug'");
	  return FALSE;
	}
      if (fnstart != NULL)
	addto_debuglist (fnstart);
      lp = fnend;
    }
  while (fnstart != NULL);
  return TRUE;
}

static bool
onetype (void)
{
  if (imagetype != NOTYPE)
    error ("Error: More than one image type specified");
  return imagetype == NOTYPE;
}

int
align (int x)
{
  return (x < 0 ? x & ALIGNMASK : (x + 3) & ALIGNMASK);
}

/*
** 'get_number' is called to assemble a numeric parameter
*/
static bool
get_number (char *p, unsigned int *value)
{
  unsigned int size, radix;
  int digit;
  char *startp, ch;
  bool ok;
  size = 0;
  radix = 10;
  ch = *p;
  if (ch == '&')
    radix = 16;
  else if (ch == '0')
    {
      p++;
      if (tolower (*p) == 'x')
	radix = 16;
    }
  if (radix == 16)
    p++;			/* Skip '&' or '0x' */
  startp = p;
  ch = tolower (*p);
  ok = TRUE;
  while (ok && ch != '\0' && ch != 'm' && ch != 'k')
    {
      if (radix == 16 && ch >= 'a')
	digit = ch - ('a' - 10);
      else
	digit = ch - '0';

      if (digit < 0 || (radix == 10 && digit > 9)
	  || (radix == 16 && digit > 15))
	ok = FALSE;
      else
	size = size * radix + digit;
      p++;
      ch = tolower (*p);
    }
  ok = ok && p != startp;
  if (ch != '\0')
    {				/* Make sure number is formed properly */
      ok = *(p + 1) == '\0';
      if (ch == 'k')
	{			/* Followed by 'k' - Size is in 'kilo' */
	  if (size < 4194304)
	    size = size * 1024;
	  else
	    ok = FALSE;
	}
      else
	{			/* Followed by 'm' - Size is in 'mega' */
	  if (size < 4096)
	    size = size * (1024 * 1024);
	  else
	    ok = FALSE;
	}
    }
  *value = size;
  return ok;
}

/*
** 'get_option' is called to identify and act on each option on the
** command line. It recognises all the valid 'link' options although
** not all of them are supported by Drlink. It flags unsupported
** options as errors.
*/
static bool
get_option (char *tp)
{
  typedef enum
  {
    NOSUPPORT, IGNORED, OPT_ACORNMAP, OPT_AIF, OPT_AOF, OPT_BASE,
    OPT_BIN, OPT_BUFFER, OPT_CASE, OPT_CPLUS, OPT_DATA, OPT_DEBIMAGE,
    OPT_DEBUG, OPT_DUMP, OPT_EDIT, OPT_GCCAREAS, OPT_HELP, OPT_INFO,
    OPT_KEEP, OPT_LEAVEWEAK, OPT_LIB, OPT_LINKERSETS, OPT_MAP, OPT_MAPFILE, OPT_NOUNUSED,
    OPT_OUTPUT, OPT_PAGE, OPT_QUIET, OPT_RELOC, OPT_RESCAN, OPT_REVMAP,
    OPT_RMOD, OPT_STRICT, OPT_STRONG, OPT_SYMBOL, OPT_THROW, OPT_VIA, OPT_VERBOSE,
    OPT_WORKS
  } actions;

  typedef struct
  {
    const char *optname;
    unsigned char optlen;
    actions optaction;
  } option;

  const option optionlist[] = {
/* areamap */ {"area", sizeof("area")-1, OPT_MAPFILE},
/* aif */ {"ai", sizeof("ai")-1, OPT_AIF},
/* aof */ {"ao", sizeof("ao")-1, OPT_AOF},
/* acornmap */ {"aco", sizeof("aco")-1, OPT_ACORNMAP},
/* buffer */ {"buf", sizeof("buf")-1, OPT_BUFFER}, /* FIXME: undocumented */
/* bin */ {"bi", sizeof("bi")-1, OPT_BIN},
/* base */ {"b", sizeof("b")-1, OPT_BASE},
/* C++ */ {"c++", sizeof("c++")-1, OPT_CPLUS},
/* case */ {"c", sizeof("c")-1, OPT_CASE},
#ifdef DEBUG
/* dump */ {"dump", sizeof("dump")-1, OPT_DUMP},
#endif
#ifndef CROSS_COMPILE
/* debimage */ {"debi", sizeof("debi")-1, OPT_DEBIMAGE},
#endif
/* data */ {"da", sizeof("da")-1, OPT_DATA},
/* dbug */ {"db", sizeof("db")-1, NOSUPPORT},
/* debug */ {"d", sizeof("d")-1, OPT_DEBUG},
/* entry */ {"en", sizeof("en")-1, NOSUPPORT},
/* edit */ {"e", sizeof("e")-1, OPT_EDIT},
/* gcc */ {"gcc", sizeof("gcc")-1, OPT_GCCAREAS},
/* help */ {"h", sizeof("h")-1, OPT_HELP},
/* info */ {"info", sizeof("info")-1, OPT_INFO},
/* keepdebug */ {"keep", sizeof("keep")-1, OPT_KEEP},
/* keeponly */ {"k", sizeof("k")-1, OPT_NOUNUSED},
/* leaveweak */ {"leave", sizeof("leave")-1, OPT_LEAVEWEAK},
/* library */ {"lib", sizeof("lib")-1, OPT_LIB},
/* linkerset */ {"linkersets", sizeof("linkersets")-1, OPT_LINKERSETS},
/* map */ {"map", sizeof("map")-1, OPT_MAP},
/* module */ {"m", sizeof("m")-1, OPT_RMOD},
/* nounused */ {"no", sizeof("no")-1, OPT_NOUNUSED},
/* overlay */ {"ov", sizeof("ov")-1, NOSUPPORT},
/* output */ {"o", sizeof("o")-1, OPT_OUTPUT},
/* pagealign */ {"pag", sizeof("pag")-1, OPT_PAGE},
/* quiet */ {"qui", sizeof("qui")-1, OPT_QUIET},
/* rescan */ {"res", sizeof("res")-1, OPT_RESCAN},
/* revmap */ {"rev", sizeof("rev")-1, OPT_REVMAP},
/* rmf */ {"rm", sizeof("rm")-1, OPT_RMOD},
/* relocatable */ {"r", sizeof("r")-1, OPT_RELOC},
/* strict */ {"strict", sizeof("strict")-1, OPT_STRICT},
/* strong */ {"str", sizeof("str")-1, OPT_STRONG},
/* symbols */ {"s", sizeof("s")-1, OPT_SYMBOL},
#ifndef CROSS_COMPILE
/* throwback */ {"t", sizeof("t")-1, OPT_THROW},
#endif
/* via */ {"via", sizeof("via")-1, OPT_VIA},
/* verbose */ {"v", sizeof("v")-1, OPT_VERBOSE},
/* workspace */ {"w", sizeof("w")-1, OPT_WORKS},
/* xref */ {"x", sizeof("x")-1, IGNORED}
  };

  unsigned int n;
  unsigned int value;
  bool ok;
  char *op;
  ok = TRUE;
  op = tp;
  tp++;				/* Skip past '-' at start of option */
  n = 0;
  while (n < sizeof (optionlist) / sizeof (option) &&
	 strncmp (tp, optionlist[n].optname, optionlist[n].optlen) != 0)
    n++;
  if (n >= sizeof (optionlist) / sizeof (option))
    {
      error ("Error: Unrecognised option '%s'", op);
      return FALSE;
    }
  switch (optionlist[n].optaction)
    {				/* Handle individual options */
    case OPT_AIF:
      ok = onetype ();
      imagetype = AIF;
      break;
    case OPT_AOF:
      ok = onetype ();
      imagetype = AOF;
      break;
    case OPT_ACORNMAP:
      opt_acornmap = TRUE;
      break;
    case OPT_PAGE:
      opt_pagealign = TRUE;
      break;
    case OPT_BASE:
      tp = get_text ();
      if (tp != NULL)
	{
	  ok = get_number (tp, &value);
	  if (!ok)
	    error ("Error: Bad numeric value supplied after option '%s'", op);
	  else if (opt_codebase)
	    {
	      error ("Error: Code base address already supplied");
	      ok = FALSE;
	    }
	  opt_codebase = TRUE;
	  codebase = align (value);
	}
      else
	{
	  error ("Error: No code base address supplied after '%s'", op);
	  ok = FALSE;
	}
      break;
    case OPT_BIN:
      ok = onetype ();
      imagetype = BIN;
      break;
    case OPT_BUFFER:
      tp = get_text ();
      if (tp != NULL)
	{
	  ok = get_number (tp, &buffersize);
	  if (!ok)
	    error ("Error: Bad numeric value supplied after option '%s'", op);
	  else
	    buffersize = align (buffersize);
	}
      else
	{
	  error ("Error: No buffer size supplied after '%s'", op);
	  ok = FALSE;
	}
      break;
    case OPT_CASE:
      opt_case = TRUE;
      break;
    case OPT_CPLUS:
      opt_cpp = TRUE;
      break;
    case OPT_DATA:
      tp = get_text ();
      if (tp != NULL)
	{
	  ok = get_number (tp, &value);
	  if (!ok)
	    error ("Error: Bad numeric value supplied after option '%s'", op);
	  else if (opt_database)
	    {
	      error ("Error: Data base address already supplied");
	      ok = FALSE;
	    }
	  opt_database = TRUE;
	  database = align (value);
	}
      else
	{
	  error ("Error: No data base address supplied after '%s'", op);
	  ok = FALSE;
	}
      break;
#ifndef CROSS_COMPILE
    case OPT_DEBIMAGE:
      opt_debimage = TRUE;
      break;
#endif
    case OPT_DEBUG:
      opt_keepdebug = opt_debug = TRUE;
      break;
#ifdef DEBUG
    case OPT_DUMP:
      opt_dump = TRUE;
      break;
#endif
    case OPT_EDIT:
      tp = get_text ();
      if (tp != NULL)
	ok = load_editfile (tp) && scan_editfile ();
      else
	{
	  error ("Error: No filename found after option '%s'", op);
	  ok = FALSE;
	}
      break;
    case OPT_GCCAREAS:
      opt_gccareas = TRUE;
      break;
    case OPT_HELP:
      print_help ();
      ok = FALSE;
      break;
    case OPT_INFO:
      opt_info = TRUE;
      break;
    case OPT_KEEP:
      tp = get_text ();
      if (tp != NULL)
	{
	  ok = get_debugfiles (tp);
	  opt_keepdebug = TRUE;
	}
      else
	{
	  error ("Error: No debug file list found after option '%s'", op);
	  ok = FALSE;
	}
      break;
    case OPT_LEAVEWEAK:
      opt_leaveweak = TRUE;
      break;
    case OPT_LIB:
      tp = get_text ();
      if (tp != NULL)
	{
	  ok = get_libraries (tp);
	}
      else
	{
	  error ("Error: No library names found after option '%s'", op);
	  ok = FALSE;
	}
      break;
    case OPT_LINKERSETS:
      opt_linkersets = TRUE;
      break;
    case OPT_MAP:
      opt_areamap = TRUE;
      break;
    case OPT_MAPFILE:
      tp = get_text ();
      if (tp != NULL)
	{
	  mapfilename = tp;
	  opt_mapfile = TRUE;
	}
      else
	{
	  error ("Error: No map file name found after option '%s'", op);
	  ok = FALSE;
	}
      break;
    case OPT_NOUNUSED:
      opt_nounused = TRUE;
      break;
    case OPT_OUTPUT:
      tp = get_text ();
      if (tp != NULL)
	imagename = tp;
      else
	{
	  ok = FALSE;
	  error ("Error: No filename found after option '%s'", op);
	}
      break;
    case OPT_QUIET:
      opt_quiet = TRUE;
      break;
    case OPT_RELOC:
      if (imagetype > AIF)
	{
	  error ("Error: More than one image type specified");
	  ok = FALSE;
	}
      imagetype = RELOC;
      break;
    case OPT_RESCAN:
      opt_rescan = TRUE;
      break;
    case OPT_REVMAP:
      opt_acornmap = opt_revmap = TRUE;
      break;
    case OPT_RMOD:
      ok = onetype ();
      imagetype = RMOD;
      break;
    case OPT_STRICT:
      error ("Warning: Warnings treated as Errors");
      opt_warningerrors = TRUE;
      break;  
    case OPT_STRONG:
      opt_strongarm = TRUE;
      break;
    case OPT_SYMBOL:
      tp = get_text ();
      if (tp != NULL)
	{
	  symbolname = tp;
	  opt_symbols = TRUE;
	}
      else
	{
	  ok = FALSE;
	  error ("Error: No filename found after option '%s'", op);
	}
      break;
#ifndef CROSS_COMPILE
    case OPT_THROW:
      opt_throw = TRUE;
      break;
#endif
    case OPT_VIA:
      tp = get_text ();
      if (tp != NULL)
	{
	  if (inviafile)
	    error ("Fatal: Nested 'via' files are not allowed");
	  ok = load_viafile (tp);
	}
      else
	{
	  ok = FALSE;
	  error ("Error: No filename found after option '%s'", op);
	}
      break;
    case OPT_VERBOSE:
      opt_verbose = TRUE;
      break;
    case OPT_WORKS:
      tp = get_text ();
      if (tp != NULL)
	{
	  ok = get_number (tp, &workspace);
	  if (!ok)
	      error ("Error: Bad numeric value supplied after option '%s'", op);
	  else
	    workspace = align (workspace);
	}
      else
	{
	  error ("Error: No workspace size supplied after '%s'", op);
	  ok = FALSE;
	}
      break;
    case IGNORED:
      error ("Warning: 'link' option '%s' is not supported and is ignored",
	     op);
      break;
    case NOSUPPORT:
      error ("Fatal: 'link' option '%s' is not supported", op);
    default:
      break;
    }
  return ok;
}

/*
** 'scan_cmdline' scans the command line picking out the files to
** include in linked program and handling the various linker options.
*/
static bool
scan_cmdline (void)
{
  char *tp;
  bool ok;
  ok = TRUE;
  tp = get_text ();
  while (tp != NULL && ok)
    {
      if (*tp != '-')
	addto_linklist (tp);	/* A possible victim */
      else
	ok = get_option (tp);
      tp = get_text ();
    }
  if (ok)
    {
      ok = linklist != NULL;
      if (!ok)
	print_help ();
    }
  if (imagetype == NOTYPE)
    imagetype = (opt_codebase ? BIN : AIF);
  if (imagetype != RELOC)
    workspace = 0;
  if (imagetype != BIN && (opt_codebase || opt_database))
    {
      opt_codebase = opt_database = FALSE;
      error
	("Error: Base address of code or data can only be specified for binary image files");
      ok = FALSE;
    }
  switch (imagetype)
    {
    case RMOD:			/* Don't want debug tables in module */
      if (opt_verbose && opt_debug)
	error ("Warning: Option '-debug' ignored for relocatable modules");
      opt_debug = FALSE;
      break;
    case AOF:
      if (imagename == NULL)
	imagename = "aof";
      opt_nounused = FALSE;
    default:
      break;
    }

  if (imagename == NULL)
#if defined(CROSS_COMPILE) && defined(ENABLE_FILETYPE_FF8)
    imagename = "!RunImage,ff8";
#else
    imagename = "!RunImage";
#endif
  opt_verbose = opt_verbose && !opt_quiet;
  return ok;
}

#ifndef CROSS_COMPILE

#define DDEUtils_GetCLSize 0x42583
#define DDEUtils_GetCL 0x42584

/*
** 'GetCLSize returns the length of an extended command line if
** one exists
*/
int
GetCLSize (void)
{
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  swierror = _kernel_swi (DDEUtils_GetCLSize, &regs, &regs);
  if (swierror != NULL)
    {				/* Error - Assume DDEUtils is not loaded */
      swierror = _kernel_last_oserror ();	/* Lose SWI error logged by C library */
      return -1;
    }

  return regs.r[0];		/* Otherwise length is returned in R0 */
}

/*
** 'GetCL' returns the contents of the extended command line. No
** check is made to ensure the DDEUtils module is available: GetCLSize
** is assumed to have called to do this
*/
void
GetCL (char *cmdline)
{
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  regs.r[0] = COERCE (cmdline, int);
  swierror = _kernel_swi (DDEUtils_GetCL, &regs, &regs);
}

/*
** 'copy_cmdline' obtains a copy of the command line for use by the
** program. If there is an extended command line available (from the
** DDE utilities) it uses that, otherwise the normal command line is
** used. The command line text will be modified later.
**
** The parsed version of the parameter list is recombined into one
** character string as then the same code can be used to deal with
** the normal and extended command lines as well as 'via' files.
**
** There are two version of this routine, one specific to RISC OS and
** the other for anyone else. The RISC OS one comes first
*/
static bool
copy_cmdline (int argc, char *argv[])
{
  int n, size;
  bool extended;
  size = GetCLSize ();
  extended = size > 0;
  if (!extended)
    {				/* Extended command line not present. Use ordinary one */
      if (argc < 2)
	return FALSE;		/* Return if no parameters supplied */
      size = 0;
      for (n = 1; n < argc; n++)
	size += strlen (argv[n]) + sizeof (char);	/* Find length of parameters */
    }
  cmdbuffer = allocmem (size + sizeof (char));	/* +1 for possible null at end */
  if (cmdbuffer == NULL)
    error ("Fatal: Not enough memory to run linker");
  if (extended)
    GetCL (cmdbuffer);		/* Copy extended command line */
  else
    {
      *cmdbuffer = '\0';
      for (n = 1; n < argc; n++)
	{
	  strcat (cmdbuffer, argv[n]);
	  strcat (cmdbuffer, " ");
	}
    }
  cvptr = cmdbuffer;
  cvptrend = cmdbufend = cmdbuffer + size;
  inviafile = FALSE;
  return TRUE;
}

#else

/*
** This is the non-RISC OS version of copy_cmdline. All it does
** is bolt the command line back together again.
*/
static bool
copy_cmdline (int argc, char *argv[])
{
  int n, size;
  if (argc < 2)
    return FALSE;		/* Return if no parameters supplied */
  size = 0;
  for (n = 1; n < argc; n++)
    size += strlen (argv[n]) + sizeof (char);	/* Find length of parameters */
  cmdbuffer = allocmem (size + sizeof (char));	/* +1 for possible null at end */
  if (cmdbuffer == NULL)
    error ("Fatal: Not enough memory to run linker");
  *cmdbuffer = '\0';
  for (n = 1; n < argc; n++)
    {
      strcat (cmdbuffer, argv[n]);
      strcat (cmdbuffer, " ");
    }
  cvptr = cmdbuffer;
  cvptrend = cmdbufend = cmdbuffer + size;
  inviafile = FALSE;
  return TRUE;
}

#endif

/*
** 'startup' initialises everything at the start of the run. It
** returns TRUE if the initialisation was successful else it
** returns FALSE
*/
static bool
startup (void)
{
  link_state = STARTING;
  warnings = 0;
  errors = 0;
  opt_verbose = FALSE;		/* Don't say what is going on */
  opt_quiet = FALSE;		/* Do not keep as quiet as a mouse */
  opt_info = FALSE;		/* Do not print info at end */
  opt_throw = FALSE;		/* Do not use throwback for error messages */
  opt_case = FALSE;		/* Do not ignore symbol case */
  opt_rescan = FALSE;		/* Only scan libraries once */
  opt_debug = FALSE;		/* Do not include debug info */
  opt_keepdebug = FALSE;	/* Not even a little bit */
  opt_leaveweak = FALSE;	/* Resolve weak external refs if possible */
  opt_debimage = FALSE;		/* Create file with type 'Absolute' */
  opt_nounused = FALSE;		/* Do not strip out unused areas */
  opt_cpp = FALSE;		/* Not linking a C++ program */
  opt_gccareas = FALSE;		/* Not ignoring GCC-specific areas when removing areas */
  opt_areamap = FALSE;		/* Do not produce a map of areas in the image file */
  opt_mapfile = FALSE;		/* Do not produce file containing map of areas */
  opt_symbols = FALSE;		/* Do not produce symbol map */
  opt_acornmap = FALSE;		/* Not producing symbol map in Acorn format */
  opt_revmap = FALSE;		/* Not listing symbols <addr> <name> */
#ifdef DEBUG
  opt_dump = FALSE;		/* Do not dump linker structures */
#endif
  opt_pagealign = FALSE;	/* Do not align end of R/O areas on page boundary */
  opt_codebase = FALSE;		/* No value given for start-of-code address */
  opt_database = FALSE;		/* No value given for start-of-R/W-data address */
  opt_strongarm = FALSE;	/* Not destined to run on a StrongARM */

  got_oldlibs = FALSE;		/* No old libraries read yet */
  linklist = linklast = NULL;
  init_heap ();
  init_files ();
  init_areas ();
  init_symbols ();
  init_edit ();
  init_library ();
  return TRUE;
}
