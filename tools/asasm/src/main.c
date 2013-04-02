/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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

#include <limits.h>
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

/* AsAsm options : */
bool option_abs = false;
int option_aof = -1; /* -1 = option not specified.  */
int option_fussy = 0;
bool option_no_code_gen = false;
bool option_nowarn = false;
int option_pedantic = 0;
int option_throwback = 0;
bool option_uppercase = false;
int option_verbose = 0;

static const char *ObjFileName = NULL;
const char *SourceFileName = NULL;

static unsigned oOptionSrcCacheSize = 8;

static SyntaxInstrState_e oOptionSyntaxInstr; /* Set via options --16, --32, --arm, --thumb, --thumbx.  */
static const char *oOptionSyntaxInstrSet; /* Non-NULL when oOptionSyntaxInstr is set.  */

/* Default value of gOptionAPCS depends on chosen FPU.  We will set it at
   ParseOption_APCS() after having determined the FPU.  */
APCS_Version_e gOptionVersionAPCS;
uint32_t gOptionAPCS;

typedef struct
{
  const char *fullOptName;
  size_t minOptNameLen;
  uint32_t bitToMask;
  uint32_t bitToSet;
} APCS_Option_t;

static const APCS_Option_t oAPCSOptions[] =
{
  { "reentrant", sizeof ("reent")-1, APCS_OPT_REENTRANT, APCS_OPT_REENTRANT }, /* reentrant, reentr */
  { "nonreentrant", sizeof ("nonreent")-1, APCS_OPT_REENTRANT, 0 }, /* nonreentrant, nonreentr */
  { "pid", sizeof ("pid")-1, APCS_OPT_REENTRANT, APCS_OPT_REENTRANT }, /* pid */
  { "nopid", sizeof ("nopid")-1, APCS_OPT_REENTRANT, 0 }, /* nopid */
  { "rwpi", sizeof ("rwpi")-1, APCS_OPT_REENTRANT, APCS_OPT_REENTRANT }, /* rwpi */
  { "norwpi", sizeof ("norwpi")-1, APCS_OPT_REENTRANT, 0 }, /* norwpi */

  { "26bit", sizeof ("26")-1, APCS_OPT_32BIT, 0 },
  { "32bit", sizeof ("32")-1, APCS_OPT_32BIT, APCS_OPT_32BIT },

  { "swstackcheck", sizeof ("swst")-1, APCS_OPT_SWSTACKCHECK, APCS_OPT_SWSTACKCHECK },
  { "noswstackcheck", sizeof ("nosw")-1, APCS_OPT_SWSTACKCHECK, 0 },

  { "fpregargs", sizeof ("fpr")-1, APCS_OPT_FPREGARGS, APCS_OPT_FPREGARGS },
  { "nofpregargs", sizeof ("nofpr")-1, APCS_OPT_FPREGARGS, 0 },

  { "fpe3", sizeof ("fpe3")-1, APCS_OPT_FPE3, APCS_OPT_FPE3 },
  { "fpe2", sizeof ("fpe2")-1, APCS_OPT_FPE3, 0 },

  { "fp", sizeof ("fp")-1, APCS_OPT_FRAMEPTR, APCS_OPT_FRAMEPTR },
  { "nofp", sizeof ("nofp")-1, APCS_OPT_FRAMEPTR, 0 },

  { "pic", sizeof ("pic")-1, APCS_OPT_ROPI, APCS_OPT_ROPI },
  { "nopic", sizeof ("nopic")-1, APCS_OPT_ROPI, 0 },
  { "ropi", sizeof ("ropi")-1, APCS_OPT_ROPI, APCS_OPT_ROPI },
  { "noropi", sizeof ("noropi")-1, APCS_OPT_ROPI, 0 },

  { "softfp", sizeof ("softfp")-1, APCS_OPT_SOFTFP, APCS_OPT_SOFTFP },
  { "hardfp", sizeof ("hardfp")-1, APCS_OPT_SOFTFP, 0 },
  
  { "interwork", sizeof ("inter")-1, APCS_OPT_INTERWORK, APCS_OPT_INTERWORK },
  { "nointerwork", sizeof ("nointer")-1, APCS_OPT_INTERWORK, 0 },

  { "fpa", sizeof ("fpa")-1, APCS_OPT_VFPENDIAN, 0 },
  { "vfp", sizeof ("vfp")-1, APCS_OPT_VFPENDIAN, APCS_OPT_VFPENDIAN }
};

/**
 * Parses the APCS option.  When no APCS option has been specified, a NULL
 * value will setup the default APCS value.
 * \return true When option value couldn't be parsed.
 */
static bool
ParseOption_APCS (const char *opt)
{
  /* Default is "3/noropi/norwpi/32bit/swstackcheck/fp/nointerwork/hardfp".
   * with either /fpa or /vfp and either /fpe2 or /fpe3 depending on chosen FPU.
   * When /vfp is selected or default, /fpregargs is default.
   */
  gOptionAPCS = APCS_OPT_32BIT | APCS_OPT_SWSTACKCHECK | APCS_OPT_FRAMEPTR;
  unsigned fpuFeatures = Target_GetFPUFeatures ();
  if ((fpuFeatures & (kFPUExt_VFPv1xD | kFPUExt_VFPv2 | kFPUExt_VFPv3xD)) != 0)
    gOptionAPCS |= APCS_OPT_VFPENDIAN;
  if ((fpuFeatures & kFPUExt_FPAv2) != 0)
    gOptionAPCS |= APCS_OPT_FPE3;
  if (gOptionAPCS & APCS_OPT_VFPENDIAN)
    gOptionAPCS |= APCS_OPT_FPREGARGS;

  if (opt == NULL)
    {
      gOptionVersionAPCS = eAPCS_v3;
      return false;
    }

  if (!strncasecmp (opt, "none", sizeof ("none")-1))
    {
      gOptionVersionAPCS = eAPCS_None;
      opt += sizeof ("none")-1;
    }
  else
    {
      if (opt[0] == '3')
	{
	  gOptionVersionAPCS = eAPCS_v3;
	  ++opt;
	}
      else if (opt[0] == '/' || opt[0] == '\0')
	gOptionVersionAPCS = eAPCS_Empty;
      else
	{
	  fprintf (stderr, PACKAGE_NAME ": Unknown APCS version %s\n", opt);
	  return true;
	}
      uint32_t bitsSet = 0;
      while (opt[0] == '/')
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
		  if ((bitsSet & oAPCSOptions[i].bitToMask) != 0
		      && (gOptionAPCS & oAPCSOptions[i].bitToMask) != oAPCSOptions[i].bitToSet)
		    {
		      fprintf (stderr, PACKAGE_NAME ": Conflicting APCS specifier %.*s with an earlier specifier\n", (int)len, opt);
		      return true;
		    }
		  bitsSet |= oAPCSOptions[i].bitToMask;

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
      /* Option VFP influences the default for FPREGARGS.  */
      if ((bitsSet & (APCS_OPT_VFPENDIAN | APCS_OPT_FPREGARGS)) == APCS_OPT_VFPENDIAN)
	{
	  gOptionAPCS &= ~APCS_OPT_FPREGARGS;
	  if ((gOptionAPCS & (APCS_OPT_VFPENDIAN | APCS_OPT_SOFTFP)) == APCS_OPT_VFPENDIAN) 
	    gOptionAPCS |= APCS_OPT_FPREGARGS;
	}
      /* Specifying /softfp with /(no)fpregargs does not make much sense.  */
      if ((bitsSet & (APCS_OPT_SOFTFP | APCS_OPT_FPREGARGS)) == (APCS_OPT_SOFTFP | APCS_OPT_FPREGARGS)
          && (gOptionAPCS & APCS_OPT_SOFTFP) != 0)
	fprintf (stderr, PACKAGE_NAME ": APCS combination /softfp with /(no)fpregargs is bizar\n");
    }
  if (opt[0] != '\0')
    {
      fprintf (stderr, PACKAGE_NAME ": Failed to parse APCS specifier %s\n", opt);
      return true;
    }
  return false;
}

RegNames_e gOptionRegNames;

/**
 * Parses the REGNAMES option.
 */
static bool
ParseOption_RegNames (const char *regnames)
{
  if (regnames == NULL)
    regnames = "callstd";
  if (!strcasecmp (regnames, "none"))
    gOptionRegNames = eNone;
  else if (!strcasecmp (regnames, "callstd"))
    gOptionRegNames = eCallStd;
  else if (!strcasecmp (regnames, "all"))
    gOptionRegNames = eAll;
  else
    return true;
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
	   "-i<directory>              Search 'directory' for included assembler files.\n"
	   "-PreDefine=<value>         Predefine a value using SETA/SETS/SETL syntax.\n"
	   "-no_code_gen               No output file generated, nor 2nd assembler pass done.\n"
	   "-Uppercase                 Recognise instruction mnemonics in upper case only.\n"
	   "-Pedantic                  Display extra warnings.\n"
	   "-Verbose                   Display progress information.\n"
	   "-Fussy                     Display conversion information.\n"
#ifdef __riscos__
	   "-ThrowBack                 Throwback errors to a text editor.\n"
#endif
	   "-cpu=<target-cpu>          Select ARM CPU to target. Use \"list\" to get a full list.\n"
	   "-fpu=<target-fpu>          Select the target floating-point unit (FPU) architecture. Use \"list\" to get a full list.\n"
	   "-device=<target-device>    Select the CPU and FPU. Use \"list\" to get a full list.\n"
	   "-Depend=<file>             Write 'make' source file dependency information to 'file'.\n"
	   "-Help                      Display this help.\n"
	   "-VERsion                   Display the version number.\n"
           "-NOWarn                    Suppress all warnings.\n"
	   "-From asmfile              Source assembler file (ObjAsm compatibility).\n"
	   "-To objfile                Destination AOF file (ObjAsm compatibility).\n"
           "-NOCache                   No source caching.\n"
           "-MaxCache=<num MByte>      Maximum source cache size.\n"
	   "-ABSolute                  Accept AAsm source code.\n"
	   "-apcs <APCS options>       Specifies one or more APCS options.\n"
	   "-regnames=none             No predefined registers.\n"
           "         =callstd          Standard set of predefined registers (depending on --apcs option).\n"
           "         =all              Full set of predefined registers.\n"
	   "-elf                       Output ELF file [default].\n"
	   "-aof                       Output AOF file.\n"
           "-16                        Start processing Thumb instructions (pre-UAL syntax).\n"
           "-32                        Synonym for -arm.\n"
           "-arm                       Start processing ARM instructions (UAL syntax).\n" 
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
  if (Error_GetExitStatus () != EXIT_SUCCESS
      && gPhase >= ePassOne)
    Output_Remove ();
}


static void
set_option_aof (int writeaof)
{
  if (option_aof != -1 && option_aof != writeaof)
    {
      fprintf (stderr, PACKAGE_NAME ": Conflicting options aof and elf\n");
      exit (EXIT_FAILURE);
    }
  option_aof = writeaof;
}


/**
 * Checks if given argument matches the option and if so, return non-NULL
 * option value.
 */
static const char *
IsOptGetArg (char ***argv, const char *opt, size_t optSize, int *argcP)
{
  const char *arg = **argv;
  /* Skip single and double '-'.  */
  if (*arg == '-')
    {
      ++arg;
      if (*arg == '-')
	++arg;
    }
  if (!strncasecmp (arg, opt, optSize))
    {
      switch (arg[optSize])
	{
	  case '\0':
	    if (--*argcP)
	      return *(++*argv);
	    fprintf (stderr, PACKAGE_NAME ": Missing argument for -%s\n", opt);
	    exit (EXIT_FAILURE);

	  case '=':
	    return &arg[optSize + 1];
	}
    }
  return NULL;
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

  unsigned numFileNames = 0;
  const char *fileNames[2];
  const char *apcs = NULL;
  const char *regnames = NULL;
  const char *cpu = NULL;
  const char *fpu = NULL;
  const char *device = NULL;

  const char *val;
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
      
      if ((val = IsOptGetArg (&argv, "PD", sizeof ("PD")-1, &argc)) != NULL
	  || (val = IsOptGetArg (&argv, "PreDefine", sizeof ("PreDefine")-1, &argc)) != NULL)
        {
	  if (Input_AddPredefine (val))
            {
	     fprintf (stderr, PACKAGE_NAME ": Too many predefines\n");
	     return EXIT_FAILURE;
            }
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
      else if (!strcasecmp (arg, "nocache") || !strcasecmp (arg, "noc") || !strcasecmp (arg, "no_cache"))
	oOptionSrcCacheSize = 0;
      else if ((val = IsOptGetArg (&argv, "MaxCache", sizeof ("MaxCache")-1, &argc)) != NULL
               || (val = IsOptGetArg (&argv, "MC", sizeof ("MC")-1, &argc)) != NULL)
	{
	  char *endVal;
	  long cacheSize = strtol(val, &endVal, 10);
	  if (endVal == val || *endVal != '\0' || cacheSize > UINT_MAX)
	    {
	      fprintf (stderr, PACKAGE_NAME ": Wrong value for source cache size\n");
	      return EXIT_FAILURE;
	    }
	  oOptionSrcCacheSize = (unsigned)cacheSize;
	}
      else if (!strcasecmp (arg, "no_code_gen"))
	option_no_code_gen = true;
#ifdef __riscos__
      else if (!strcasecmp (arg, "throwback") || !strcasecmp (arg, "tb"))
	option_throwback++;
#endif
      else if (!strcasecmp (arg, "u") || !strcasecmp (arg, "uppercase"))
	option_uppercase = true;
      else if (!strcasecmp (arg, "pedantic") || !strcasecmp (arg, "p"))
	option_pedantic++;
      else if ((val = IsOptGetArg (&argv, "cpu", sizeof ("cpu")-1, &argc)) != NULL)
        {
	  if (cpu != NULL && strcasecmp (cpu, val))
	    {
	      fprintf (stderr, PACKAGE_NAME ": Option %s has already been specified: %s and as %s\n", "cpu", cpu, val);
	      return EXIT_FAILURE;
	    }
	  cpu = val;
	}
      else if ((val = IsOptGetArg (&argv, "fpu", sizeof ("fpu")-1, &argc)) != NULL)
        {
	  if (fpu != NULL && strcasecmp (fpu, val))
	    {
	      fprintf (stderr, PACKAGE_NAME ": Option %s has already been specified: %s and as %s\n", "cpu", fpu, val);
	      return EXIT_FAILURE;
	    }
	  fpu = val;
	}
      else if ((val = IsOptGetArg (&argv, "device", sizeof ("device")-1, &argc)) != NULL)
        {
	  if (device != NULL && strcasecmp (device, val))
	    {
	      fprintf (stderr, PACKAGE_NAME ": Option %s has already been specified: %s and as %s\n", "device", device, val);
	      return EXIT_FAILURE;
	    }
	  device = val;
	}
      else if (!strcasecmp (arg, "verbose") || !strcasecmp (arg, "v"))
	option_verbose++;
      else if (!strcasecmp (arg, "fussy") || !strcasecmp (arg, "f"))
	option_fussy++;
      else if ((val = IsOptGetArg (&argv, "apcs", sizeof ("apcs")-1, &argc)) != NULL)
	{
	  if (apcs != NULL && strcasecmp (apcs, val))
	    {
	      fprintf (stderr, PACKAGE_NAME ": Option %s has already been specified: %s and as %s\n", "apcs", apcs, val);
	      return EXIT_FAILURE;
	    }
	  apcs = val;
	}
      else if ((val = IsOptGetArg (&argv, "regnames", sizeof ("regnames")-1, &argc)) != NULL)
	{
	  if (regnames != NULL && strcasecmp (regnames, val))
	    {
	      fprintf (stderr, PACKAGE_NAME ": Option %s has already been specified: %s and as %s\n", "regnames", regnames, val);
	      return EXIT_FAILURE;
	    }
	  regnames = val;
	}
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
	  /* Support comma separated directories.  */
	  while (inclDir)
	    {
	      char *c = strchr (inclDir, ',');
	      if (c)
		*c++ = '\0';
	      Include_Add (inclDir);
	      inclDir = c;
	    }
	}
      else if (!strcasecmp (arg, "version") || !strcasecmp (arg, "ver"))
	{
	  fprintf (stderr,
		   DEFAULT_IDFN "\n"
		   "Copyright (c) 1992-2013 Niklas Röjemo, Darren Salt and GCCSDK Developers\n"
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
      else if (!strcasecmp (arg, "nowarn") || !strcasecmp (arg, "now"))
	option_nowarn = true;
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
      else if ((val = IsOptGetArg (&argv, "d", sizeof ("d")-1, &argc)) != NULL
	       || (val = IsOptGetArg (&argv, "Depend", sizeof ("Depend")-1, &argc)) != NULL)
	{
	  if (DependFileName != NULL && strcasecmp (DependFileName, val))
	    {
	      fprintf (stderr, PACKAGE_NAME ": option %s has already been specified: %s and as %s\n", "depend", DependFileName, val);
	      return EXIT_FAILURE;
	    }
	  DependFileName = val;
	}
      else if (!strcasecmp (arg, "absolute") || !strcasecmp (arg, "abs"))
	option_abs = true;
      else if (!strcasecmp (arg, "elf"))
	set_option_aof (0);
      else if (!strcasecmp (arg, "aof"))
	set_option_aof (1);
      else if (!strcasecmp (arg, "16"))
	{
	  if (oOptionSyntaxInstr != eState_Code16 && oOptionSyntaxInstrSet != NULL)
	    {
	      fprintf (stderr, PACKAGE_NAME ": option %s conflicts with previous given option %s\n", arg, oOptionSyntaxInstrSet);
	      return EXIT_FAILURE;
	    }
	  oOptionSyntaxInstr = eState_Code16;
	  oOptionSyntaxInstrSet = arg;
	}
      else if (!strcasecmp (arg, "32") || !strcasecmp (arg, "arm"))
	{
	  if (oOptionSyntaxInstr != eState_Code32 && oOptionSyntaxInstrSet != NULL)
	    {
	      fprintf (stderr, PACKAGE_NAME ": option %s conflicts with previous given option %s\n", arg, oOptionSyntaxInstrSet);
	      return EXIT_FAILURE;
	    }
	  oOptionSyntaxInstr = eState_Code32;
	  oOptionSyntaxInstrSet = arg;
	}
      else if (!strcasecmp (arg, "thumb"))
	{
	  if (oOptionSyntaxInstr != eState_Thumb && oOptionSyntaxInstrSet != NULL)
	    {
	      fprintf (stderr, PACKAGE_NAME ": option %s conflicts with previous given option %s\n", arg, oOptionSyntaxInstrSet);
	      return EXIT_FAILURE;
	    }
	  oOptionSyntaxInstr = eState_Thumb;
	  oOptionSyntaxInstrSet = arg;
	}
      else if (!strcasecmp (arg, "thumbx"))
	{
	  if (oOptionSyntaxInstr != eState_ThumbEE && oOptionSyntaxInstrSet != NULL)
	    {
	      fprintf (stderr, PACKAGE_NAME ": option %s conflicts with previous given option %s\n", arg, oOptionSyntaxInstrSet);
	      return EXIT_FAILURE;
	    }
	  oOptionSyntaxInstr = eState_ThumbEE;
	  oOptionSyntaxInstrSet = arg;
	}
      else if (!strcasecmp (arg, "stamp") || !strcasecmp (arg, "quit"))
	{
	  /* -stamp & -quit are old AAsm/ObjAsm options which we silently
	     ignore.  */
	}
      else
	fprintf (stderr, PACKAGE_NAME ": Unknown option -%s ignored\n", arg);
    }

  if (option_aof == -1)
    option_aof = 0;
	    
  if (Target_SetCPU_FPU_Device (cpu, fpu, device))
    return EXIT_FAILURE;
  /* The FPU option needs to be processed first before we can process apcs
     as the FPU will determine the default APCS value /fpa vs /vfp and
     /fpe2 vs /fpe3.  */
  if (ParseOption_APCS (apcs))
    return EXIT_FAILURE;
  if (ParseOption_RegNames (regnames))
    return EXIT_FAILURE;

  /* Pass on --16, --32, --arm, --thumb, --thumbx options.  */
  if (oOptionSyntaxInstrSet == NULL)
    oOptionSyntaxInstr = Target_CheckCPUFeature (kArchExt_v1, false) ? eState_Code32 : eState_Thumb; 
  State_Set (oOptionSyntaxInstr);

  FS_SetFileCacheSize (oOptionSrcCacheSize);
  
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

      /* Initialize and do the one or two assembly passes.  */
      Phase_PrepareFor (eStartUp);
      ASM_Assemble (SourceFileName, option_no_code_gen);

      /* Don't try to output anything when we have assemble errors.  */
      const char *canonOutFileName;
      if (Error_GetExitStatus () == EXIT_SUCCESS
          && (canonOutFileName = Output_OpenOutput (ObjFileName)) != NULL)
	{
	  if (option_verbose)
	    fprintf (stderr, "Writing %s file at %s\n", option_aof ? "AOF" : "ELF", canonOutFileName);
	  if (setjmp (asmContinue))
	    fprintf (stderr, PACKAGE_NAME ": Error when writing object file '%s'.\n", canonOutFileName);
	  else
	    {
	      asmContinueValid = true;
	      /* Write the ELF/AOF output.  */
	      Phase_PrepareFor (eOutput);
	      if (!option_aof)
		Output_ELF ();
	      else
		Output_AOF ();
	    }
	  asmContinueValid = false;
	}
    }

  Phase_PrepareFor (eCleanUp);
  return Error_GetExitStatus ();
}
