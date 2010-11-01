/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#ifdef __riscos__
#  include <kernel.h>
#endif

#include "area.h"
#include "asm.h"
#include "error.h"
#include "filestack.h"
#include "include.h"
#include "input.h"
#include "main.h"
#include "os.h"
#include "output.h"
#include "symbol.h"
#include "targetcpu.h"
#include "depend.h"
#include "variables.h"

jmp_buf asmContinue;
bool asmContinueValid = false;
jmp_buf asmAbort;
bool asmAbortValid = false;

/* AS options :
 */
int option_verbose = 0;
int option_pedantic = 0;
int option_fussy = 0;
int option_throwback = 0;
int option_autocast = 0;
int option_align = 1;
int option_local = 1;
int option_apcs_32bit = -1; /* -1 = option not specified.  */
int option_apcs_fpv3 = -1; /* -1 = option not specified.  */
int option_apcs_softfloat = -1; /* -1 = option not specified.  */
int option_rma_module = 0;
int option_aof = -1; /* -1 = option not specified.  */

const char *predefines[MAX_PREDEFINES];
int num_predefines = 0;


const char *ProgName = NULL;
static const char *ObjFileName = NULL;
const char *SourceFileName = NULL;


static void
as_help (void)
{
  fprintf (stderr,
	   DEFAULT_IDFN
	   "\n"
	   "Usage: %s [option]... <asmfile> <objfile>\n"
	   "       %s [option]... -o <objfile> <asmfile>\n"
	   "\n"
	   "Options:\n"
	   "-o objfile                 Specifies destination AOF/ELF file.\n"
	   "-I<directory>              Search 'directory' for included assembler files.\n"
	   "-D<variable>               Define a string variable.\n"
	   "-D<variable>=<value>       Define a string variable to a certain value.\n"
	   "-PD <value>                Predefine a value using SETA/SETS/SETL syntax.\n"
	   "-PreDefine <value>         Same as -PD option.\n"
	   "-pedantic      -p          Display extra warnings.\n"
	   "-verbose       -v          Display progress information.\n"
	   "-fussy         -f          Display conversion information.  Can be specified more than once for more conversion information.\n"
#ifdef __riscos__
	   "-throwback     -tb         Throwback errors to a text editor.\n"
#endif
	   "-autocast      -ac         Enable casting from integer to float.\n"
	   "-target        -t          Target ARM CPU (ARM2...SA110).\n"
	   "-depend <file> -d <file>   Write 'make' source file dependency information to 'file'.\n"
	   "-noalign       -na         Don't auto-align words and halfwords.\n"
	   "-nolocal       -nl         No builtin LOCAL support.\n"
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
	   ProgName, ProgName);
}

static bool finished = false;

static void
atexit_handler (void)
{
  if (!finished || returnExitStatus () != EXIT_SUCCESS)
    outputRemove ();
}

static void
set_option_apcs_32bit (int write32bit)
{
  if (option_apcs_32bit != -1 && option_apcs_32bit != write32bit)
    {
      fprintf (stderr, "%s: Conflicting options -apcs26 and -apcs32\n", ProgName);
      exit (EXIT_FAILURE);
    }
  option_apcs_32bit = write32bit;
}

static void
set_option_apcs_fpv3 (int writefpv3)
{
  if (option_apcs_fpv3 != -1 && option_apcs_fpv3 != writefpv3)
    {
      fprintf (stderr, "%s: Conflicting options -apcsfpv2 and -apcsfpv3\n", ProgName);
      exit (EXIT_FAILURE);
    }
  option_apcs_fpv3 = writefpv3;
}

static void
set_option_apcs_softfloat(int writesf)
{
  if (option_apcs_softfloat != -1 && option_apcs_softfloat != writesf)
    {
      fprintf (stderr, "%s: Conflicting options -soft-float and -hard-float\n", ProgName);
      exit (EXIT_FAILURE);
    }
  option_apcs_softfloat = writesf;
}

static void
set_option_aof (int writeaof)
{
  if (option_aof != -1 && option_aof != writeaof)
    {
      fprintf (stderr, "%s: Conflicting options -aof and -elf\n", ProgName);
      exit (EXIT_FAILURE);
    }
  option_aof = writeaof;
}

int
main (int argc, char **argv)
{
  atexit (atexit_handler);

  setlocale (LC_ALL, "");

  ProgName = *argv++;

#define IS_ARG(ln, sn) (!strcmp(*argv, ln) || !strcmp(*argv, sn))

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
		  fprintf (stderr, "%s: Missing argument after -D\n", ProgName);
		  return EXIT_FAILURE;
		}
	    }
	  else
	    var_define (argv[0] + 2);
	}
      else if (IS_ARG ("-PD", "-PreDefine"))
        {
          if (--argc)
            {
              if (num_predefines == MAX_PREDEFINES)
                {
		  fprintf (stderr, "%s: Too many predefines\n", ProgName);
		  return EXIT_FAILURE;
                }

              predefines[num_predefines++] = *++argv;
            }
          else
	    {
              fprintf (stderr, "%s: Missing argument after -PD/-PreDefine\n", ProgName);
	      return EXIT_FAILURE;
	    }
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
	option_throwback++;
#endif
      else if (IS_ARG ("-autocast", "-ac"))
	option_autocast++;
      else if (IS_ARG ("-noalign", "-na"))
	option_align = 0;
      else if (IS_ARG ("-nolocal", "-nl"))
	option_local = 0;
      else if (IS_ARG ("-pedantic", "-p"))
	option_pedantic++;
      else if (IS_ARG ("-target", "-t"))
        {
	  if (as_target (--argc ? *++argv : NULL) < 0)
	    return EXIT_FAILURE;
	}
      else if (IS_ARG ("-verbose", "-v"))
	option_verbose++;
      else if (IS_ARG ("-fussy", "-f"))
	option_fussy++;
      else if (!strcmp (*argv, "-apcs26"))
	set_option_apcs_32bit (0);
      else if (!strcmp (*argv, "-apcs32"))
	set_option_apcs_32bit (1);
      else if (!strcmp (*argv, "-apcsfpv2"))
	set_option_apcs_fpv3 (0);
      else if (!strcmp (*argv, "-apcsfpv3"))
	set_option_apcs_fpv3 (1);
      else if (!strcmp (*argv, "-soft-float"))
	set_option_apcs_softfloat (1);
      else if (!strcmp (*argv, "-hard-float"))
	set_option_apcs_softfloat (0);
      else if (!strcmp (*argv, "-module"))
	option_rma_module = 1;
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
	  fprintf (stderr,
	           DEFAULT_IDFN "\n"
	           "Copyright (c) 1992-2010 Niklas Rojemo, Darren Salt and GCCSDK Developers\n");
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
#ifndef NO_ELF_SUPPORT
      else if (!strcmp (*argv, "-elf"))
	set_option_aof (0);
#endif
      else if (!strcmp (*argv, "-aof"))
	set_option_aof (1);
      else if (**argv != '-')
	{
	  if (SourceFileName != NULL)
	    {
	      if (ObjFileName != NULL)
		{
		  fprintf (stderr, "%s: Only one input file allowed\n", ProgName);
		  return EXIT_FAILURE;
		}
	      ObjFileName = *argv;
	    }
	  else
	    SourceFileName = *argv;
	}
      else
	fprintf (stderr, "%s: Illegal flag %s ignored\n", ProgName, *argv);
    }

  /* Fallback on default options ? */
  if (option_apcs_32bit == -1)
    option_apcs_32bit = 1;
  if (option_apcs_fpv3 == -1)
    option_apcs_fpv3 = 1;
  if (option_apcs_softfloat == -1)
    option_apcs_softfloat = 0;
  if (option_aof == -1)
#ifndef NO_ELF_SUPPORT
    option_aof = 0;
#else
    option_aof = 1;
#endif

  set_cpuvar ();

  if (SourceFileName == NULL)
    {
      fprintf (stderr, "%s: No input filename specified\n", ProgName);
      return EXIT_FAILURE;
    }
  if (ObjFileName == NULL)
    {
      fprintf (stderr, "%s: No output filename specified\n", ProgName);
      return EXIT_FAILURE;
    }
  
  /* When the command line has been sorted, get on with the job in hand */

  if (setjmp (asmAbort))
    {
      asmAbortValid = false;
      fprintf (stderr, "%s: Aborted\n", ProgName);
      while (gCurPObjP != NULL)
	FS_PopPObject (true);
    }
  else
    {
      asmAbortValid = true;
      symbolInit ();
      inputInit (SourceFileName);

      /* ... do the assembly ... */
      outputInit (ObjFileName);

      /* ... tidy up and write the ELF/AOF output.  */
      areaInit ();
      setjmp (asmContinue); asmContinueValid = true;
      assemble ();
      areaFinish ();
      
      if (setjmp (asmContinue))
	fprintf (stdout, "%s: Error when writing object file '%s'.\n", ProgName, ObjFileName);
      else
        {
#ifndef NO_ELF_SUPPORT
	  if (!option_aof)
	    outputElf();
	  else
#endif
	    outputAof();
	}
    }
  outputFinish ();
  errorFinish ();
  finished = true; /* No longer enforce removing output file.  */
  return returnExitStatus ();
}
