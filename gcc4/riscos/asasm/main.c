/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2012 GCCSDK Developers
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
#include "common.h"
#include "depend.h"
#include "error.h"
#include "filestack.h"
#include "include.h"
#include "input.h"
#include "main.h"
#include "local.h"
#include "output.h"
#include "phase.h"
#include "predef_reg.h"
#include "state.h"
#include "symbol.h"
#include "targetcpu.h"
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
int option_apcs_softfloat = -1; /* -1 = option not specified.  */
int option_aof = -1; /* -1 = option not specified.  */
bool option_abs = false;
bool option_uppercase = false;

const char *predefines[MAX_PREDEFINES];
int num_predefines = 0;

static const char *ObjFileName = NULL;
const char *SourceFileName = NULL;

bool gIsAPCS = true;
uint8_t gOptionAPCS = APCS_OPT_32BIT | APCS_OPT_SWSTACKCHECK | APCS_OPT_FRAMEPTR;

typedef struct
{
  const char *fullOptName;
  size_t minOptNameLen;
  uint8_t bitToMask;
  uint8_t bitToSet;
} APCS_Option_t;

static const APCS_Option_t oAPCSOptions[] =
{
  { "reentrant", sizeof ("reent")-1, APCS_OPT_REENTRANT, APCS_OPT_REENTRANT },
  { "nonreentrant", sizeof ("nonreent")-1, APCS_OPT_REENTRANT, 0 },

  { "26bit", sizeof ("26")-1, APCS_OPT_32BIT, 0 },
  { "32bit", sizeof ("32")-1, APCS_OPT_32BIT, APCS_OPT_32BIT },

  { "swstackcheck", sizeof ("swst")-1, APCS_OPT_SWSTACKCHECK, APCS_OPT_SWSTACKCHECK },
  { "noswstackcheck", sizeof ("noswst")-1, APCS_OPT_SWSTACKCHECK, 0 },

  { "fpregargs", sizeof ("fpr")-1, APCS_OPT_FPREGARGS, APCS_OPT_FPREGARGS },
  { "nofpregargs", sizeof ("nofpr")-1, APCS_OPT_FPREGARGS, 0 },

  { "fpe3", sizeof ("fpe3")-1, APCS_OPT_FPE3, APCS_OPT_FPE3 },
  { "fpe2", sizeof ("fpe2")-1, APCS_OPT_FPE3, 0 },

  { "fp", sizeof ("fp")-1, APCS_OPT_FRAMEPTR, APCS_OPT_FRAMEPTR },
  { "nofp", sizeof ("nofp")-1, APCS_OPT_FRAMEPTR, 0 }
};

static bool
APCS_ParseOption (const char *opt)
{
  if (!strcasecmp (opt, "none"))
    gIsAPCS = false;
  else
    {
      gIsAPCS = true;
      if (opt[0] != '3' || (opt[1] != '\0' && opt[1] != '/'))
	{
	  fprintf (stderr, PACKAGE_NAME ": Unknown APCS version %s\n", opt);
	  return true;
	}
      ++opt;
      while (opt[0] == '/' && opt[1] != '\0')
	{
	  ++opt;
	  const char *slash = strchr (opt, '/');
	  size_t len = slash ? (size_t)(slash - opt) : strlen (opt);
	  size_t i;
	  for (i = 0; i != sizeof (oAPCSOptions)/sizeof (oAPCSOptions[0]); ++i)
	    {
	      if (!strncasecmp (opt, oAPCSOptions[i].fullOptName, len)
	          && len >= oAPCSOptions[i].minOptNameLen)
		{
		  /* This option is selected.  */
		  gOptionAPCS = (gOptionAPCS & ~oAPCSOptions[i].bitToMask) | oAPCSOptions[i].bitToSet;
		  opt += len;
		  break;
		}
	    }
	  if (i == sizeof (oAPCSOptions)/sizeof (oAPCSOptions[0]))
	    {
	      fprintf (stderr, PACKAGE_NAME ": Unknown APCS 3 specifier %.*s\n", (int)len, opt);
	      return true;
	    }
	}
    }
  return false;
}

static void
asasm_help (void)
{
  fprintf (stderr,
	   DEFAULT_IDFN
	   "\n"
	   "Usage: " PACKAGE_NAME " [option]... <asmfile> <objfile>\n"
	   "       " PACKAGE_NAME " [option]... -o <objfile> <asmfile>\n"
	   "\n"
	   "Options:\n"
	   "-o objfile                 Specifies destination AOF/ELF file.\n"
	   "-I<directory>              Search 'directory' for included assembler files.\n"
	   "-D<variable>               Define a string variable.\n"
	   "-D<variable>=<value>       Define a string variable to a certain value.\n"
	   "-PreDefine <value>         Predefine a value using SETA/SETS/SETL syntax.\n"
	   "-UpperCase                 Recognise instruction mnemonics in upper case only.\n"
	   "-Pedantic                  Display extra warnings.\n"
	   "-Verbose                   Display progress information.\n"
	   "-Fussy                     Display conversion information.\n"
#ifdef __riscos__
	   "-ThrowBack                 Throwback errors to a text editor.\n"
#endif
	   "-CPU <target-cpu>          Select ARM CPU to target. Use \"list\" to get a full list.\n"
	   "-Depend <file>             Write 'make' source file dependency information to 'file'.\n"
	   "-Help                      Display this help.\n"
	   "-VERsion                   Display the version number.\n"
	   "-From asmfile              Source assembler file (ObjAsm compatibility).\n"
	   "-To objfile                Destination AOF file (ObjAsm compatibility).\n"
	   "-ABSolute                  Accept AAsm source code.\n"
	   "-Apcs <APCS options>       Specifies one or more APCS options.\n"
	   "-soft-float                Mark code as using -msoft-float (avoids explicit FP instructions).  This is a GCCSDK extension to the AOF file format.\n"
	   "-hard-float                Mark code as using -mhard-float (uses explicit FP instructions) [default].\n"
#ifndef NO_ELF_SUPPORT
	   "-elf                       Output ELF file [default].\n"
#endif
	   "-aof                       Output AOF file.\n"
           "-16                        Start processing Thumb instructions (pre-UAL syntax).\n"
           "-32                        Synonym for -arm.\n"
           "-arm                       Start processing ARM instructions.\n" 
           "-thumb                     Start processing Thumb instructions (UAL syntax).\n"
           "-thumbx                    Start processing ThumbEE instructions (UAL syntax).\n"
	   "\n");
}

static void
atexit_handler (void)
{
  /* Only remove the output file when there was an error and we actually
     started doing the assembling (i.e. don't remove the output file
     when there was an option error).  */
  if (returnExitStatus () != EXIT_SUCCESS
      && gPhase >= ePassOne)
    Output_Remove ();
}

static void
set_option_apcs_softfloat(int writesf)
{
  if (option_apcs_softfloat != -1 && option_apcs_softfloat != writesf)
    {
      fprintf (stderr, PACKAGE_NAME ": Conflicting options -soft-float and -hard-float\n");
      exit (EXIT_FAILURE);
    }
  option_apcs_softfloat = writesf;
}

static void
set_option_aof (int writeaof)
{
  if (option_aof != -1 && option_aof != writeaof)
    {
      fprintf (stderr, PACKAGE_NAME ": Conflicting options -aof and -elf\n");
      exit (EXIT_FAILURE);
    }
  option_aof = writeaof;
}


int
main (int argc, char **argv)
{
  atexit (atexit_handler);

  setlocale (LC_ALL, "");

#ifdef DEBUG
  setvbuf(stdout, NULL, _IOLBF, 0);
#endif

  argv++;
  if (argc == 1)
    {
      /* No command line arguments supplied. Print help and exit.  */
      asasm_help ();
      return EXIT_SUCCESS;
    }
  /* Analyse the command line */

  int numFileNames = 0;
  const char *fileNames[2];
  const char *cpu = NULL;
  const char *apcs = NULL;
  
  for (argc--; argc; argv++, argc--)
    {
      const char *arg = *argv;
      if (arg[0] != '-')
	{
	  /* This is not an option.  */
	  if (numFileNames == 2)
	    {
	      fprintf (stderr, PACKAGE_NAME ": Too many filenames specified\n");
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
		Var_Define (*++argv);
	      else
		{
		  fprintf (stderr, PACKAGE_NAME ": Missing argument after -%s\n", arg);
		  return EXIT_FAILURE;
		}
	    }
	  else
	    Var_Define (arg + 1);
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
              fprintf (stderr, PACKAGE_NAME ": Missing argument after -%s\n", arg);
	      return EXIT_FAILURE;
	    }
	  else
	    val = *++argv;
	    
          if (num_predefines == MAX_PREDEFINES)
            {
	     fprintf (stderr, PACKAGE_NAME ": Too many predefines\n");
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
		  fprintf (stderr, PACKAGE_NAME ": Only one output file allowed\n");
		  return EXIT_FAILURE;
		}
	      ObjFileName = *++argv;
	    }
	  else
	    {
	      fprintf (stderr, PACKAGE_NAME ": Missing filename after -%s\n", arg);
	      return EXIT_FAILURE;
	    }
	}
#ifdef __riscos__
      else if (!strcasecmp (arg, "throwback") || !strcasecmp (arg, "tb"))
	option_throwback++;
#endif
      else if (!strcasecmp (arg, "uc") || !strcasecmp (arg, "uppercase"))
	option_uppercase = true;
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
              fprintf (stderr, PACKAGE_NAME ": Missing argument after -%s\n", arg);
	      return EXIT_FAILURE;
	    }
	  else
	    val = *++argv;
	  if (cpu != NULL && strcasecmp (cpu, val))
	    {
	      fprintf (stderr, PACKAGE_NAME ": CPU is specified twice: %s and %s\n", cpu, val);
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
              fprintf (stderr, PACKAGE_NAME ": Missing argument after -%s\n", arg);
	      return EXIT_FAILURE;
	    }
	  else
	    val = *++argv;

	  apcs = val;
	}
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
	          fprintf(stderr, PACKAGE_NAME ": Missing include directory after -%s\n", arg);
	          return EXIT_FAILURE;
	        }
	      inclDir = *++argv;
	    }
	  Include_Add (inclDir);
	}
      else if (!strcasecmp (arg, "version") || !strcasecmp (arg, "ver"))
	{
	  fprintf (stderr,
		   DEFAULT_IDFN "\n"
		   "Copyright (c) 1992-2012 Niklas Röjemo, Darren Salt and GCCSDK Developers\n"
		   "This is free software; see the source for copying conditions.  There is NO\n"
		   "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
	  return EXIT_SUCCESS;
	}
      else if (!strcasecmp (arg, "H") || !strcasecmp (arg, "help") || !strcasecmp (arg, "?"))
	{
	  /* We need the `--help' option for gcc's --help -v  */
	  asasm_help ();
	  return EXIT_SUCCESS;
	}
      else if (!strcasecmp (arg, "From"))
	{
	  if (--argc)
	    {
	      if (SourceFileName != NULL)
	        {
	          fprintf (stderr, PACKAGE_NAME ": Only one input file allowed (%s & %s specified)\n", SourceFileName, *++argv);
	          return EXIT_FAILURE;
	        }
	      SourceFileName = *++argv;
	    }
	  else
	    {
	      fprintf (stderr, PACKAGE_NAME ": Missing filename after -%s\n", arg);
	      return EXIT_FAILURE;
	    }
	}
      else if (!strcasecmp (arg, "depend") || !strcasecmp (arg, "d"))
	{
	  if (--argc)
	    {
	      if (DependFileName != NULL)
	        {
	          fprintf (stderr, PACKAGE_NAME ": Only one dependency file allowed (%s & %s specified)\n", DependFileName, *++argv);
	          return EXIT_FAILURE;
	        }
	      DependFileName = *++argv;
	    }
	  else
	    {
	      fprintf (stderr, PACKAGE_NAME ": Missing filename after -%s\n", arg);
	      return EXIT_FAILURE;
	    }
	}
      else if (!strcasecmp (arg, "absolute") || !strcasecmp (arg, "abs"))
	option_abs = true;
#ifndef NO_ELF_SUPPORT
      else if (!strcasecmp (arg, "elf"))
	set_option_aof (0);
#endif
      else if (!strcasecmp (arg, "aof"))
	set_option_aof (1);
      else if (!strcasecmp (arg, "16"))
	{
	  State_SetCmdLineInstrType (eInstrType_Thumb);
	  State_SetCmdLineSyntax (eSyntax_PreUALOnly);
	}
      else if (!strcasecmp (arg, "32") || !strcasecmp (arg, "arm"))
	{
	  State_SetCmdLineInstrType (eInstrType_ARM);
	  State_SetCmdLineSyntax (eSyntax_Both);
	}
      else if (!strcasecmp (arg, "thumb"))
	{
	  State_SetCmdLineInstrType (eInstrType_Thumb);
	  State_SetCmdLineSyntax (eSyntax_UALOnly);
	}
      else if (!strcasecmp (arg, "thumbx"))
	{
	  State_SetCmdLineInstrType (eInstrType_ThumbEE);
	  State_SetCmdLineSyntax (eSyntax_UALOnly);
	}
      else if (!strcasecmp (arg, "stamp") || !strcasecmp (arg, "quit"))
	{
	  /* -stamp & -quit are old AAsm/ObjAsm options which we silently
	     ignore.  */
	}
      else
	fprintf (stderr, PACKAGE_NAME ": Unknown option -%s ignored\n", arg);
    }

  /* Fallback on default options ? */
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
  if (apcs != NULL && APCS_ParseOption (apcs))
    return EXIT_FAILURE;

  if (ObjFileName == NULL)
    {
      if (numFileNames)
	ObjFileName = fileNames[--numFileNames];
      else
	{
	  fprintf (stderr, PACKAGE_NAME ": No output filename specified\n");
	  return EXIT_FAILURE;
	}
    }
  if (SourceFileName == NULL)
    {
      if (numFileNames)
	SourceFileName = fileNames[--numFileNames];
      else
	{
	  fprintf (stderr, PACKAGE_NAME ": No input filename specified\n");
	  return EXIT_FAILURE;
	}
    }
  if (numFileNames)
    {
      while (numFileNames)
        fprintf (stderr, PACKAGE_NAME ": Specified unused filename: %s\n", fileNames[--numFileNames]);
      return EXIT_FAILURE;
    }
  
  if (setjmp (asmAbort))
    {
      asmAbortValid = false;
      fprintf (stderr, PACKAGE_NAME ": Aborted\n");
      while (gCurPObjP != NULL)
	FS_PopPObject (true);
    }
  else
    {
      asmAbortValid = true;
      PreDefReg_Init ();
      Output_Init (ObjFileName);

      /* Do the two pass assembly.  */
      ASM_Assemble (SourceFileName);

      /* Don't try to output anything when we have assemble errors.  */
      if (returnExitStatus () == EXIT_SUCCESS)
	{
	  if (setjmp (asmContinue))
	    fprintf (stderr, PACKAGE_NAME ": Error when writing object file '%s'.\n", ObjFileName);
	  else
	    {
	      asmContinueValid = true;
	      /* Write the ELF/AOF output.  */
	      Phase_PrepareFor (eOutput);
	      if (returnExitStatus () == EXIT_SUCCESS)
		{
#ifndef NO_ELF_SUPPORT
		  if (!option_aof)
		    Output_ELF ();
		  else
#endif
		    Output_AOF ();
		}
	    }
	  asmContinueValid = false;
	}
    }
  Output_Finish ();
  errorFinish ();
  return returnExitStatus ();
}
