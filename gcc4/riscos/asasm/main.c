/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2011 GCCSDK Developers
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
#include <strings.h>
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
int option_apcs_32bit = -1; /* -1 = option not specified.  */
int option_apcs_fpv3 = -1; /* -1 = option not specified.  */
int option_apcs_softfloat = -1; /* -1 = option not specified.  */
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
	   "-PreDefine <value>         Predefine a value using SETA/SETS/SETL syntax.\n"
	   "-Pedantic                  Display extra warnings.\n"
	   "-Verbose                   Display progress information.\n"
	   "-Fussy                     Display conversion information.  Can be specified more than once for more conversion information.\n"
#ifdef __riscos__
	   "-ThrowBack                 Throwback errors to a text editor.\n"
#endif
	   "-AutoCast                  Enable casting from integer to float.\n"
	   "-CPU <target-cpu>          Select ARM CPU to target. Use \"list\" to get a full list.\n"
	   "-Depend <file>             Write 'make' source file dependency information to 'file'.\n"
	   "-Help                      Display this help.\n"
	   "-VERsion                   Display the version number.\n"
	   "-From asmfile              Source assembler file (ObjAsm compatibility).\n"
	   "-To objfile                Destination AOF file (ObjAsm compatibility).\n"
	   "-Apcs <APCS options>       Specifies one or more APCS options.\n"
	   "-apcs26                    26-bit APCS AREAs.\n"
	   "-apcs32                    32-bit APCS AREAs [default].\n"
	   "-apcsfpv2                  Use floating point v2 AREAs.\n"
	   "-apcsfpv3                  Use floating point v3 AREAs (SFM, LFM) [default]\n"
	   "-soft-float                Mark code as using -msoft-float (avoids explicit FP instructions).  This is a GCCSDK extension to the AOF file format.\n"
	   "-hard-float                Mark code as using -mhard-float (uses explicit FP instructions) [default].\n"
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

  if (argc == 1)
    {
      /* No command line arguments supplied. Print help and exit.  */
      as_help ();
      return EXIT_SUCCESS;
    }
  /* Analyse the command line */

  int numFileNames = 0;
  const char *fileNames[2];
  const char *cpu = NULL;
  
  for (argc--; argc; argv++, argc--)
    {
      const char *arg = *argv;
      if (arg[0] != '-')
	{
	  /* This is not an option.  */
	  if (numFileNames == 2)
	    {
	      fprintf (stderr, "%s: Too many filenames specified\n", ProgName);
	      return EXIT_FAILURE;
	    }
	  fileNames[numFileNames++] = arg;
	  continue;
	}
      ++arg;

      /* Accept single & double dash for options.  */
      if (arg[0] == '-')
	++arg;
      
      if (arg[0] == 'D')
	{
	  if (arg[1] == '\0')
	    {
	      if (--argc)
		var_define (*++argv);
	      else
		{
		  fprintf (stderr, "%s: Missing argument after -%s\n", ProgName, arg);
		  return EXIT_FAILURE;
		}
	    }
	  else
	    var_define (arg + 1);
	}
      else if (!strncasecmp (arg, "PD", sizeof ("PD")-1)
	       || !strncasecmp (arg, "PreDefine", sizeof ("PreDefine")-1))
        {
	  const char *val;
	  if (arg[sizeof ("PD")-1] == '=')
	    val = arg + sizeof ("PD")-1 + 1;
	  else if (arg[sizeof ("PreDefine")-1] == '=')
	    val = arg + sizeof ("PreDefine")-1 + 1;
	  else if (--argc == 0)
	    {
              fprintf (stderr, "%s: Missing argument after -%s\n", ProgName, arg);
	      return EXIT_FAILURE;
	    }
	  else
	    val = *++argv;
	    
          if (num_predefines == MAX_PREDEFINES)
            {
	     fprintf (stderr, "%s: Too many predefines\n", ProgName);
	     return EXIT_FAILURE;
            }
          predefines[num_predefines++] = val;
        }
      else if (!strcasecmp (arg, "o") || !strcasecmp (arg, "To"))
	{
	  if (--argc)
	    {
	      if (ObjFileName != NULL)
		{
		  fprintf (stderr, "%s: Only one output file allowed\n", ProgName);
		  return EXIT_FAILURE;
		}
	      ObjFileName = *++argv;
	    }
	  else
	    {
	      fprintf (stderr, "%s: Missing filename after -%s\n", ProgName, arg);
	      return EXIT_FAILURE;
	    }
	}
#ifdef __riscos__
      else if (!strcasecmp (arg, "throwback") || !strcasecmp (arg, "tb"))
	option_throwback++;
#endif
      else if (!strcasecmp (arg, "autocast") || !strcasecmp (arg, "ac"))
	option_autocast++;
      else if (!strcasecmp (arg, "pedantic") || !strcasecmp (arg, "p"))
	option_pedantic++;
      else if (!strncasecmp (arg, "CPU", sizeof ("CPU")-1)
	       && (arg[sizeof ("CPU")-1] == '=' || arg[sizeof ("CPU")-1] == '\0'))
        {
	  const char *val;
	  if (arg[sizeof ("CPU")-1] == '=')
	    val = arg + sizeof ("CPU")-1 + 1;
	  else if (--argc == 0)
	    {
              fprintf (stderr, "%s: Missing argument after -%s\n", ProgName, arg);
	      return EXIT_FAILURE;
	    }
	  else
	    val = *++argv;
	  if (cpu != NULL && strcasecmp (cpu, val))
	    {
	      fprintf (stderr, "%s: CPU is specified twice: %s and %s\n", ProgName, cpu, val);
	      return EXIT_FAILURE;
	    }
	  cpu = val;
	}
      else if (!strcasecmp (arg, "verbose") || !strcasecmp (arg, "v"))
	option_verbose++;
      else if (!strcasecmp (arg, "fussy") || !strcasecmp (arg, "f"))
	option_fussy++;
      else if ((!strncasecmp (arg, "apcs", sizeof ("apcs")-1)
	        && (arg[sizeof ("apcs")-1] == '=' || arg[sizeof ("apcs")-1] == '\0'))
		 || ((arg[0] == 'a' || arg[1] == 'A')
		     && (arg[sizeof ("a")-1] == '=' || arg[sizeof ("A")-1] == '\0')))
	{
	  const char *val;
	  if (arg[sizeof ("apcs")-1] == '=')
	    val = arg + sizeof ("apcs")-1 + 1;
	  else if (arg[sizeof ("a")-1] == '=')
	    val = arg + sizeof ("a")-1 + 1;
	  else if (--argc == 0)
	    {
              fprintf (stderr, "%s: Missing argument after -%s\n", ProgName, arg);
	      return EXIT_FAILURE;
	    }
	  else
	    val = *++argv;

	  fprintf (stderr, "%s: Warning: APCS option not implemented\n", ProgName);
	  /* FIXME */
	}
      else if (!strcasecmp (arg, "apcs26"))
	set_option_apcs_32bit (0);
      else if (!strcasecmp (arg, "apcs32"))
	set_option_apcs_32bit (1);
      else if (!strcasecmp (arg, "apcsfpv2"))
	set_option_apcs_fpv3 (0);
      else if (!strcasecmp (arg, "apcsfpv3"))
	set_option_apcs_fpv3 (1);
      else if (!strcasecmp (arg, "soft-float"))
	set_option_apcs_softfloat (1);
      else if (!strcasecmp (arg, "hard-float"))
	set_option_apcs_softfloat (0);
      else if (arg[0] == 'I' || arg[0] == 'i')
	{
	  const char *inclDir = arg + 1;
	  if (*inclDir == '\0')
	    {
	      if (--argc == 0)
	        {
	          fprintf(stderr, "%s: Missing include directory after -%s\n", ProgName, arg);
	          return EXIT_FAILURE;
	        }
	      inclDir = *++argv;
	    }
	  if (addInclude (inclDir) < 0)
	    return EXIT_FAILURE;
	}
      else if (!strcasecmp (arg, "version") || !strcasecmp (arg, "ver"))
	{
	  fprintf (stderr,
		   DEFAULT_IDFN "\n"
		   "Copyright (c) 1992-2011 Niklas Rojemo, Darren Salt and GCCSDK Developers\n"
		   "This is free software; see the source for copying conditions.  There is NO\n"
		   "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
	  return EXIT_SUCCESS;
	}
      else if (!strcasecmp (arg, "H") || !strcasecmp (arg, "help") || !strcasecmp (arg, "?"))
	{
	  /* We need the `--help' option for gcc's --help -v  */
	  as_help ();
	  return EXIT_SUCCESS;
	}
      else if (!strcasecmp (arg, "From"))
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
	      fprintf (stderr, "%s: Missing filename after -%s\n", ProgName, arg);
	      return EXIT_FAILURE;
	    }
	}
      else if (!strcasecmp (arg, "depend") || !strcasecmp (arg, "d"))
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
	      fprintf (stderr, "%s: Missing filename after -%s\n", ProgName, arg);
	      return EXIT_FAILURE;
	    }
	}
#ifndef NO_ELF_SUPPORT
      else if (!strcasecmp (arg, "elf"))
	set_option_aof (0);
#endif
      else if (!strcasecmp (arg, "aof"))
	set_option_aof (1);
      else
	fprintf (stderr, "%s: Unknown option -%s ignored\n", ProgName, arg);
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
	    
  if (Target_SetCPU (cpu ? cpu : "arm7tdmi"))
    return EXIT_FAILURE;

  if (SourceFileName == NULL)
    {
      if (numFileNames)
	SourceFileName = fileNames[--numFileNames];
      else
	{
	  fprintf (stderr, "%s: No input filename specified\n", ProgName);
	  return EXIT_FAILURE;
	}
    }
  if (ObjFileName == NULL)
    {
      if (numFileNames)
	ObjFileName = fileNames[--numFileNames];
      else
	{
	  fprintf (stderr, "%s: No output filename specified\n", ProgName);
	  return EXIT_FAILURE;
	}
    }
  if (numFileNames)
    {
      while (numFileNames)
        fprintf (stderr, "%s: Specified unused filename: %s\n", ProgName, fileNames[--numFileNames]);
      return EXIT_FAILURE;
    }
  
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
      outputInit (ObjFileName);
      areaInit ();
      /* Do the assembly.  */
      ASM_Assemble (SourceFileName);
      areaFinish ();

      /* Don't try to output anything when we have assemble errors.  */
      if (returnExitStatus () == EXIT_SUCCESS)
	{
	  if (setjmp (asmContinue))
	    fprintf (stderr, "%s: Error when writing object file '%s'.\n", ProgName, ObjFileName);
	  else
	    {
	      asmContinueValid = true;
	      /* Write the ELF/AOF output.  */
#ifndef NO_ELF_SUPPORT
	      if (!option_aof)
		outputElf();
	      else
#endif
		outputAof();
	    }
	  asmContinueValid = false;
	}
    }
  outputFinish ();
  errorFinish ();
  finished = true; /* No longer enforce removing output file.  */
  return returnExitStatus ();
}
