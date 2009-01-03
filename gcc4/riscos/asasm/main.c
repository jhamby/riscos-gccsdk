/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2008 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * main.c
 */

#include "config.h"
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
#ifdef __riscos__
#include <kernel.h>
#endif

#include "area.h"
#include "asm.h"
#include "error.h"
#include "include.h"
#include "input.h"
#include "main.h"
#include "os.h"
#include "output.h"
#include "targetcpu.h"
#include "version.h"
#ifdef __riscos__
#include "depend.h"
#endif
#include "variables.h"

jmp_buf asmContinue;
BOOL asmContinueValid = FALSE;
jmp_buf asmAbort;
BOOL asmAbortValid = FALSE;

/* AS options :
 */
int verbose = 0;
int pedantic = 0;
int fussy = 0;
int throwback = 0;
int dde = 0;
int autocast = 0;
int align = 1;
int local = 1;
int objasm = 0;
int uc = 0;
int apcs_32bit = -1;
int apcs_fpv3 = -1;
int apcs_softfloat = -1;
int rma_module = 0;
static int option_elf = 0;
static int option_aof = 0;

const char *ProgName = NULL;
const char *ObjFileName = NULL;
const char *SourceFileName = NULL;

static void
as_help (void)
{
  fprintf (stderr,
	   "AS AOF"
#ifndef NO_ELF_SUPPORT
	   "/ELF"
#endif
	   " Assembler " VERSION " (" __DATE__ ") [GCCSDK]"
	   "\n"
	   "Usage: %s [option]... <asmfile>\n"
	   "\n"
	   "Options:\n"
	   "-o objfile                 Specifies destination AOF file.\n"
	   "-Idirectory                Search 'directory' for included assembler files.\n"
	   "-pedantic      -p          Display extra warnings.\n"
	   "-verbose       -v          Display progress information.\n"
	   "-fussy         -f          Display conversion information.  Can be specified more than once for more conversion information.\n"
	   "-dde                       Replace '@' in filenames with <Prefix$Dir>.\n"
#ifdef __riscos__
	   "-throwback     -tb         Throwback errors to a text editor.\n"
#endif
	   "-autocast      -ac         Enable casting from integer to float.\n"
	   "-target        -t          Target ARM CPU (ARM2...SA110).\n"
#ifdef __riscos__
	   "-depend <file> -d <file>   Write 'make' source file dependency information to 'file'.\n"
#endif
	   "-noalign       -na         Don't auto-align words and halfwords.\n"
	   "-nolocal       -nl         No builtin LOCAL support.\n"
	   "-objasm        -obj        More compatibility with ObjAsm.\n"
	   "-upper         -up         Mnemonics must be in upper case.\n"
	   "-help          -h -H -?    Display this help.\n"
	   "-version       -ver        Display the version number.\n"
	   "-From asmfile              Source assembler file (ObjAsm compatibility).\n"
	   "-To objfile                Destination AOF file (ObjAsm compatibility).\n"
	   "-apcs26                    26-bit APCS AREAs.\n"
	   "-apcs32                    32-bit APCS AREAs [default].\n"
	   "-apcsfpv2                  Use floating point v2 AREAs.\n"
	   "-apcsfpv3                  Use floating point v3 AREAs (SFM, LFM) [default]\n"
	   "-soft-float                Mark code as using -msoft-float (avoids explicit FP instructions).  This is a GCCSDK extension to the AOF file format.\n"
	   "-hard-float                Mark code as using -mhard-float (uses explicit FP instructions) [default].\n"
	   "-module                    Set if building RISC OS RMA modules.\n"
#ifndef NO_ELF_SUPPORT
	   "-elf                       Output ELF file [default].\n"
#endif
	   "-aof                       Output AOF file.\n"
	   "\n",
	   ProgName);
}


#ifdef __riscos__
static char *prefix;
#endif

static int finished = 0;


static void
restore_prefix (void)
{
  if (!finished || returnExitStatus () != EXIT_SUCCESS)
    outputRemove ();
#ifdef __riscos__
  if (prefix)
    _kernel_setenv ("Prefix$Dir", prefix);
#endif
  /* workaround for throwback/Prefix$Dir problem */
}

int main (int argc, char **argv)
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
  ProgName = *argv++;

#define IS_ARG(ln,sn) !strcmp(*argv,ln) || !strcmp(*argv,sn)

  if (argc == 1)
    {
      /* No command line arguments supplied. Print help and exit.  */
      as_help ();
      return EXIT_SUCCESS;
    }
  /* Analyse the command line */

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
		  return EXIT_FAILURE;
		}
	    }
	  else
	    var_define (argv[0] + 2);
	}
      else if (IS_ARG ("-o", "-To"))
	{
	  if (ObjFileName != NULL)
	    {
	      fprintf (stderr, "%s: Only one output file allowed\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  if (--argc)
	    ObjFileName = *++argv;
	  else
	    {
	      fprintf (stderr, "%s: Missing filename after -o\n", ProgName);
	      return EXIT_FAILURE;
	    }
	}
#ifdef __riscos__
      else if (IS_ARG ("-throwback", "-tb"))
	throwback++;
#endif
      else if (IS_ARG ("-autocast", "-ac"))
	autocast++;
      else if (IS_ARG ("-noalign", "-na"))
	align = 0;
      else if (IS_ARG ("-nolocal", "-nl"))
	local = 0;
      else if (IS_ARG ("-objasm", "-obj"))
	{
	  /* Used as index in lex.c.  */
	  objasm = 1;
	}
      else if (IS_ARG ("-upper", "-up"))
	uc++;
      else if (IS_ARG ("-pedantic", "-p"))
	pedantic++;
      else if (IS_ARG ("-target", "-t"))
        {
	  if (as_target (--argc ? *++argv : NULL) < 0)
	    return EXIT_FAILURE;
	}
      else if (IS_ARG ("-verbose", "-v"))
	verbose++;
      else if (IS_ARG ("-fussy", "-f"))
	fussy++;
      else if (! strcmp (*argv, "-apcs26"))
        {
          if (apcs_32bit == 1)
            {
	      fprintf (stderr, "%s: Conflicting options -apcs26 and -apcs32\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  apcs_32bit = 0;
	}
      else if (! strcmp (*argv, "-apcs32"))
        {
          if (apcs_32bit == 0)
            {
	      fprintf (stderr, "%s: Conflicting options -apcs26 and -apcs32\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  apcs_32bit = 1;
	}
      else if (! strcmp (*argv, "-apcsfpv2"))
        {
          if (apcs_fpv3 == 1)
            {
	      fprintf (stderr, "%s: Conflicting options -apcsfpv2 and -apcsfpv3\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  apcs_fpv3 = 0;
	}
      else if (! strcmp (*argv, "-apcsfpv3"))
        {
          if (apcs_fpv3 == 0)
            {
	      fprintf (stderr, "%s: Conflicting options -apcsfpv2 and -apcsfpv3\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  apcs_fpv3 = 1;
	}
      else if (! strcmp (*argv, "-soft-float"))
        {
          if (apcs_softfloat == 0)
            {
              fprintf (stderr, "%s: Conflicting options -soft-float and -hard-float\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  apcs_softfloat = 1;
	}
      else if (! strcmp (*argv, "-hard-float"))
        {
          if (apcs_softfloat == 1)
            {
              fprintf (stderr, "%s: Conflicting options -soft-float and -hard-float\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  apcs_softfloat = 0;
	}
      else if (! strcmp (*argv, "-module"))
	rma_module = 1;
      else if (!strcmp (*argv, "-dde"))
	dde++;
      else if (!strncmp (*argv, "-I", 2))
	{
	  const char *inclDir = *argv + 2;
	  if (*inclDir == '\0')
	    {
	      if (--argc == 0)
	        {
	          fprintf(stderr, "%s: Missing include directory after -I\n", ProgName);
	          return EXIT_FAILURE;
	        }
	      inclDir = *++argv;
	    }
	  if (addInclude (inclDir) < 0)
	    return EXIT_FAILURE;
	}
      else if (IS_ARG ("-version", "-ver"))
	{
	  fprintf (stderr, "AS AOF"
#ifndef NO_ELF_SUPPORT
		   "/ELF"
#endif
		   " Assembler " VERSION " (" __DATE__ ") [GCCSDK]\n");

	  fprintf (stderr, "Copyright (c) 1992-2008 Niklas Rojemo, Darren Salt and GCCSDK Developers\n");
	  return EXIT_SUCCESS;
	}
      else if (IS_ARG ("-H", "-h")
	       || IS_ARG ("-help", "-?")
	       || !strcmp (*argv, "--help"))
	{
	  /* We need the `--help' option for gcc's --help -v  */
	  as_help ();
	  return EXIT_SUCCESS;
	}
      else if (!strcmp (*argv, "-From"))
	{
	  if (--argc)
	    {
	      if (SourceFileName != NULL)
	        {
	          fprintf (stderr, "%s: Only one input file allowed (%s & %s specified)\n", ProgName, SourceFileName, *++argv);
	          return EXIT_FAILURE;
	        }
	      SourceFileName = *++argv;
	    }
	  else
	    {
	      fprintf (stderr, "%s: Missing filename after -From\n", ProgName);
	      return EXIT_FAILURE;
	    }
	}
#ifdef __riscos__
      else if (IS_ARG ("-depend", "-d"))
	{
	  if (--argc)
	    {
	      if (DependFileName != NULL)
	        {
	          fprintf (stderr, "%s: Only one dependency file allowed (%s & %s specified)\n", ProgName, DependFileName, *++argv);
	          return EXIT_FAILURE;
	        }
	      DependFileName = *++argv;
	    }
	  else
	    {
	      fprintf (stderr, "%s: Missing filename after -depend\n", ProgName);
	      return EXIT_FAILURE;
	    }
	}
#endif
#ifndef NO_ELF_SUPPORT
      else if (!strcmp (*argv, "-elf"))
	option_elf++;
#endif
      else if (!strcmp (*argv, "-aof"))
	option_aof++;
      else if (**argv != '-')
	{
	  if (SourceFileName != NULL)
	    {
	      fprintf (stderr, "%s: Only one input file allowed (%s & %s specified)\n", ProgName, SourceFileName, *argv);
	      return EXIT_FAILURE;
	    }
	  SourceFileName = *argv;
	}
      else
	fprintf (stderr, "%s: Illegal flag %s ignored\n", ProgName, *argv);
    }

  /* Fallback on default options ? */
  if (apcs_32bit == -1)
    apcs_32bit = 1;
  if (apcs_fpv3 == -1)
    apcs_fpv3 = 1;
  if (apcs_softfloat == -1)
    apcs_softfloat = 0;

  set_cpuvar();

  /* When the command line has been sorted, get on with the job in hand */
  if (ObjFileName == NULL)
    ObjFileName = SourceFileName;

  if (setjmp (asmAbort))
    {
      asmAbortValid = FALSE;
      fprintf (stderr, "%s: Aborted\n", ProgName);
      inputFinish ();
    }
  else
    {
      asmAbortValid = TRUE;
      inputInit (SourceFileName);
      errorInit (SourceFileName);
#ifdef __riscos__
      dependInit (SourceFileName);
#endif

      /* ... do the assembly ... */
      outputInit (ObjFileName);

      /* ... tidy up and write the AOF */
      areaInit ();
      setjmp (asmContinue); asmContinueValid = TRUE;
      assemble ();
      areaFinish ();
      inputFinish ();
      if (setjmp (asmContinue))
	fprintf (stdout, "%s: Error when writing object file '%s'.\n", ProgName, ObjFileName);
      else
        {
#ifndef NO_ELF_SUPPORT
	  if (option_elf && option_aof)
	    {
	      fprintf (stderr, "%s: Conflicting options -aof and -elf\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  /* Writing ELF output format is default.  */
	  if (!option_aof)
	    outputElf();
	  else
#endif
	    outputAof();
	}
#ifdef __riscos__
      dependPut ("\n", "", "");
#endif
    }
  outputFinish ();
#ifdef __riscos__
  dependFinish ();
#endif
  errorFinish ();
  finished = 1;
  return returnExitStatus ();
}
