
/*
 *  main.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "main.h"
#include "input.h"
#include "output.h"
#include "error.h"
#include "asm.h"
#include "area.h"
#include "os.h"
#include "include.h"
#include "version.h"
#include "targetcpu.h"
#ifndef CROSS_COMPILE
#include "depend.h"
#endif
#include "variables.h"

jmp_buf asmContinue;
jmp_buf asmAbort;

int verbose = 0;
int pedantic = 0;
int fussy = 0;
int throwback = 0;
int dde = 0;
int autocast = 0;
int align = 1;
int gcc_backend = 1;
int gas_backend = 0;
int local = 1;
int objasm = 0;
int uc = 0;
int apcs_32bit = 1;
int apcs_softfloat = 0;
int apcs_fpv3 = 1;

char *ProgName;
char *ObjFileName;
char *SourceFileName;

static void
as_help (char *progname)
{
  fprintf (stderr,
	   "%s"
	   "\n"
	   "Usage: %s [option]... <asmfile>\n"
	   "\n"
	   "Options:\n"
	   "-o objfile        Destination AOF file\n"
       "-Idirectory       Search 'directory' for included assembler files\n"
	   "-pedantic   -p    Display extra warnings\n"
	   "-verbose    -v    Display progress information\n"
	   "-fussy      -f    Display conversion information\n"
	   "-dde              Replace '@' in filenames with <Prefix$Dir>\n"
	   "-throwback  -tb   Throwback errors to a text editor\n"
	   "-autocast   -ac   Enable casting from integer to float\n"
	   "-target     -t    Target ARM CPU (ARM2...SA110)\n"
	   "-gas              Support some GNU GAS keywords\n"
	   "-noalign    -na   Don't auto-align words and halfwords\n"
	   "-nolocal    -nl   No builtin LOCAL support\n"
	   "-objasm     -obj  More compatibility with ObjAsm\n"
	   "-upper      -up   Mnemonics must be in upper case\n"
	   "-help -h -H -?    Display this help\n"
	   "-version    -ver  Display the version number\n"
	   "-From asmfile     Source assembler file (Objasm compatibility)\n"
	   "-To objfile       Destination AOF file (Objasm compatibility)\n"
	   "-apcs26           26-bit APCS AREAs\n"
	   "-apcs32           32-bit APCS AREAs (default)\n"
	   "-apcsfpv2         Use floating point v2 AREAs\n"
	   "-apcsfpv3         Use floating point v3 AREAs (SFM, LFM) (default)\n"
	   "\n",
	   VERSION, progname);
}


#ifdef __riscos__
extern int _kernel_setenv (const char *, const char *);
static char *prefix;
#else
static const char *DependFileName;
#endif

static int finished = 0;


static void
restore_prefix (void)
{
  if (!finished || noerrors ())
    outputRemove ();
#ifdef __riscos__
  if (prefix)
    _kernel_setenv ("Prefix$Dir", prefix);
#endif
  /* workaround for throwback/Prefix$Dir problem */
}


int
main (int argc, char **argv)
{
#ifdef __riscos__
  ProgName = getenv ("Prefix$Dir");
  /* There's a strange problem with Prefix$Dir becoming unset if
   * throwback is used...
   */
  prefix = ProgName ? strdup (ProgName) : 0;
#endif
  atexit (restore_prefix);
  setlocale (LC_ALL, "");
  initInclude ();
  ProgName = *argv++;
  ObjFileName = NULL;

#define IS_ARG(ln,sn) !strcmp(*argv,ln) || !strcmp(*argv,sn)

  if (argc == 1)
    {
      /* No command line arguments supplied. Print help and exit.  */
      as_help (ProgName);
      return 0;
    }

  for (argc--; argc; argv++, argc--)
    {
      if (argv[0][0] == '-' && argv[0][1] == 'D')
	{
	  if (argv[0][2] == 0)
	    {
	      if (--argc)
		var_define (*++argv);
	      else
		{
		  fprintf (stderr, "%s: Missing argument after -o\n", ProgName);
		  return -1;
		}
	    }
	  else
	    var_define (argv[0] + 2);
	}
      else if (IS_ARG ("-o", "-To"))
	{
	  if (ObjFileName)
	    {
	      fprintf (stderr, "%s: Only one output file allowed\n", ProgName);
	      return -1;
	    }
	  if (--argc)
	    ObjFileName = *++argv;
	  else
	    {
	      fprintf (stderr, "%s: Missing filename after -o\n", ProgName);
	      return -1;
	    }
	}
      else if (IS_ARG ("-throwback", "-tb"))
	throwback++;
      else if (IS_ARG ("-autocast", "-ac"))
	autocast++;
      else if (IS_ARG ("-noalign", "-na"))
	align = 0;
      else if (IS_ARG ("-nolocal", "-nl"))
	local = 0;
      else if (IS_ARG ("-objasm", "-obj"))
	objasm = 1;		/* used as index in lex.c */
      else if (IS_ARG ("-upper", "-up"))
	uc++;
      else if (IS_ARG ("-pedantic", "-p"))
	pedantic++;
      else if (IS_ARG ("-target", "-t"))
	as_target (--argc ? *++argv : 0);
      else if (IS_ARG ("-verbose", "-v"))
	verbose++;
      else if (IS_ARG ("-fussy", "-f"))
	fussy++;
      else if (! strcmp (*argv, "-apcs26"))
	apcs_32bit = 0;
      else if (! strcmp (*argv, "-apcs32"))
	apcs_32bit++;
      else if (! strcmp (*argv, "-apcsfpv2"))
	apcs_fpv3 = 0;
      else if (! strcmp (*argv, "-apcsfpv3"))
	apcs_fpv3++;
      else if (!strcmp (*argv, "-gas"))
	gas_backend++;
      else if (!strcmp (*argv, "-dde"))
	dde++;
      else if (!strncmp (*argv, "-I", 2))
	{
	  if (addInclude (*argv + 2) < 0)
	    return 1;
	}
      else if (IS_ARG ("-version", "-ver"))
	{
	  fprintf (stderr, "%s%s", VERSION, COPYRIGHT);
	  return 0;
	}
      else if (IS_ARG ("-H", "-h")
	       || IS_ARG ("-help", "-?")
	       || !strcmp (*argv, "--help"))
	{
	  /* We need the `--help' option for gcc's --help -v  */
	  as_help (ProgName);
	  return 0;
	}
      else if (!strcmp (*argv, "-From"))
	{
	  if (SourceFileName)
	    {
	      fprintf (stderr, "%s: Only one input file allowed\n", ProgName);
	      return -1;
	    }
	  if (--argc)
	    SourceFileName = *++argv;
	  else
	    {
	      fprintf (stderr, "%s: Missing filename after -From\n", ProgName);
	      return -1;
	    }
	}
      else if (IS_ARG ("-depend", "-d"))
	{
	  if (DependFileName)
	    {
	      fprintf (stderr, "%s: Only one dependency file allowed\n", ProgName);
	      return -1;
	    }
	  if (--argc)
	    DependFileName = *++argv;
	  else
	    {
	      fprintf (stderr, "%s: Missing filename after -depend\n", ProgName);
	      return -1;
	    }
	}
      else if (**argv != '-')
	{
	  if (SourceFileName)
	    {
	      fprintf (stderr, "%s: Only one input file allowed\n", ProgName);
	      return -1;
	    }
	  SourceFileName = *argv;
	}
      else
	fprintf (stderr, "%s: Illegal flag %s ignored\n", ProgName, *argv);
    }
  if (!ObjFileName)
    ObjFileName = SourceFileName;

  if (setjmp (asmAbort))
    {
      fprintf (stderr, "%s: Aborted\n", ProgName);
      inputFinish ();
      outputFinish ();
    }
  else
    {
      inputInit (SourceFileName);
      errorInit (SourceFileName);
      outputInit (ObjFileName);
      areaInit ();
      setjmp (asmContinue);
      asm_ ();
      areaFinish ();
      inputFinish ();
      if (setjmp (asmContinue))
	fprintf (stdout, "%s: Error when writing object file '%s'.\n", ProgName, ObjFileName);
      else
	outputAof ();
#ifdef __riscos__
      dependPut ("\n", "", "");
#endif
      outputFinish ();
      errorFinish ();
    }
  finished = 1;
  return noerrors ();
}
