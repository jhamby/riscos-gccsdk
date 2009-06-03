/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gfile.h"
#include "error.h"
#include "options.h"
#include "str.h"
#include "mem.h"
#include "format.h"
#include "apcscli.h"

/* Instantiate the global options struct here */
options opt;

void Options_Init(void) {

  opt.runnable    = run_none;
  opt.apcs        = DEFAULT_APCS;
  opt.apcs_used   = 0;
  opt.toolchain_used = 0;
  opt.toolchain   = tc_norcroft;
  opt.no_scl      = 0;
  opt.reentrant   = 1;
  opt.cplusplus   = 0;
  opt.service     = NULL;
  opt.services    = NULL;
  opt.title       = NULL;
  opt.help        = NULL;
  opt.helpfn      = NULL;
  opt.commands    = NULL;
  opt.command_codesupplied = 0;
  opt.swi_base    = 0;
  opt.swi_handler = NULL;
  opt.swi_names   = NULL;
  opt.swi_codesupplied = 0;
  opt.swi_decoder = NULL;
  opt.pdriver_entry = NULL;
  opt.pdriver_names = NULL;
  opt.error_base  = 0;
  opt.irqs        = NULL;
  opt.events      = NULL;
  opt.init        = NULL;
  opt.lib_init    = NULL;
  opt.final       = NULL;
  opt.generics    = NULL;
  opt.vectors     = NULL;
  opt.vector_traps = NULL; /* vectors which can be post processed */
  opt.errors      = NULL;
  opt.errors_special = 0;
  opt.version     = 0;
  opt.date        = NULL;
  opt.datestring  = NULL;
  opt.blank       = 0;
  opt.ofile       = NULL; /* AOF or ELF output */
  opt.sfile       = NULL; /* Assembler output */
  opt.stemp       = 0;
  opt.hfile       = NULL; /* Header output */
  opt.x_hdr       = NULL;
  opt.x_h         = NULL;
  opt.mfile       = NULL;
  opt.dfile       = NULL;
  opt.oslib       = 0;
  opt.base        = 0;
  opt.mode_errors = 0; /* We're not in 'errors' mode */
  opt.throwback   = 0;
  opt.infile      = NULL;
  opt.atline      = 0;
  opt.rootinfile  = NULL;
  opt.pfile       = NULL; /* Pre-processor output */
  opt.pextended   = 0;    /* Extended pre-processor input */
  opt.defines     = NULL;
  opt.includes    = NULL;
  opt.cmhg        = 0; /* We're not CMHG */
  opt.help_requested = 0;
}

static void help_text(void) {

  format_wrap(stdout,"","",
"\n"
"Make a RISC OS module header for a module written in C/C++.\n"
"\n"
#ifdef __riscos
"Syntax: *CMunge [<options>] <infile>\n"
#else
/* Non-RISC OS systems don't use the above style for their usage commands. */
"Usage: cmunge [options] infile\n"
#endif
"\n"
"Options:\n"
"\n"
"\t-h            \bOutput this information.\n"
"\t-o <file>     \bName the output AOF or ELF file.\n"
"\t-s <file>     \bName the output assembler file.\n"
"\t-d <file>     \bName a C header file that CMunge will generate.\n"
"\t-xhdr <file>  \bName a Assembler SWI header file that CMunge will generate.\n"
"\t-xh <file>    \bName a C SWI header file that CMunge will generate.\n"
"\t-p            \bPre-process the file with pre-processor before parsing.\n"
"\t-px           \bExtended pre-processing of all content from pre-processor.\n"
/*"-b            \bUse _blib_entermodule instead of SCL version\n" */
"\t-I<directory> \bInclude <directory> on the #include search path.\n"
"\t-D<symbol>=<value>\b\n"
"\t-D<symbol>    \bDefine <symbol> on entry to the preprocessor.\n"
"\t-U<symbol>    \bUndefine <symbol> on entry to the preprocessor.\n"
"\t-depend <file>\bOutput 'make' include file dependency information to <file>.\n"
"\t-throwback    \bUse throwback for error processing.\n"
"\t-zbase        \bDefine symbol Image__RO_Base in generated header.\n"
"\t-zerrors      \bDefine errors and veneers only (for use in applications).\n"
"\t-zoslib       \bUse 'os_error *' instead of '_kernel_oserror *' to prototype "
                 "a pointer to a RISC OS error block, 'os_register_block' "
                 "instead of '_kernel_swi_regs' to represent a set of ARM "
                 "registers, '#include \"oslib/os.h\"' (OSLib) instead of "
                 "'#include \"kernel.h\"' (SCL) in generated C header "
                 "file.\n"
"\t-znoscl       \bDon't interface with Shared C Library, i.e. don't have "
                 "runtime support.\n"
"\t-blank        \bGenerate a blank cmhg file to infile.\n"
"\t-cmhg         \bGive warnings for non-CMHG values.\n"
"\t-tgcc         \bUse GCC tool chain to generate output.\n"
"\t-tlcc         \bUse LCC tool chain to generate output.\n"
"\t-tnorcroft    \bUse Norcroft tool chain to generate output (default).\n"
"\t-32bit        \bGenerate 32-bit compatible code.\n"
"\t-26bit        \bGenerate 26-bit only code (default).\n"
"\t-apcs 3/<flags>\bSpecify APCS variant to use.\n",
"");

  /* Display the APCS variant in use */
  apcscli_help(opt.apcs, 1);
}

void Options_CL(int argc, char *argv[]) {

  int i;
  defines_list define;
  include_list inc, *inc_tail;
  char *arg;
  char *val;

  inc_tail = &opt.includes;
  for (i = 1; i < argc; i++) {
    if (stricmp("-throwback", argv[i])) {
      opt.throwback=1;
    } else if (strncmp("-t", argv[i], sizeof("-t")-1)==0) {
      char *arg = &argv[i][2];
      if (stricmp("norcroft", arg)) {
        opt.toolchain = tc_norcroft;
      } else if (stricmp("gcc", arg)) {
        opt.toolchain = tc_gcc;
      } else if (stricmp("lcc", arg)) {
        opt.toolchain = tc_lcc;
      }
      else
        ErrorFatal("Toolchain %s not known!",arg);
      opt.toolchain_used = 1;
    } else if (strncmp("-x", argv[i], sizeof("-x")-1)==0) {
      char *arg = &argv[i][2];
      char **file = NULL; /* Couldn't find opt */
      if (stricmp("hdr", arg)) {
        file=&opt.x_hdr;
      } else if (stricmp("h", arg)) {
        file=&opt.x_h;
      }
      if (file==NULL)
        ErrorFatal("Export flag %s not known!",arg);
      if (*file)
        ErrorFatal("Export flag %s used twice!",arg);
      i++;
      if (i < argc) {
        *file = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with export flag %s flag!",arg);
      }
    } else if (stricmp("-h", argv[i]) || stricmp("-help", argv[i])) {
      opt.help_requested = 1;
    } else if (stricmp("-p", argv[i]) || stricmp("-px", argv[i])) {
      if (opt.pfile) {
        ErrorFatal("-p flag used twice!");
      }
      opt.pfile=file_temp();
      if (opt.pfile==NULL)
        ErrorFatal("Failed to find a temporary file for preprocessor");
      if (stricmp("-px", argv[i]))
        opt.pextended = 1;
    } else if (stricmp("-cmhg", argv[i])) {
      if (opt.cmhg) {
        ErrorFatal("-cmhg flag used twice!");
      }
      opt.cmhg=1;
    } else if (stricmp("-s", argv[i])) {
      if (opt.sfile) {
        ErrorFatal("-s flag used twice!");
      }
      i++;
      if (i < argc) {
        opt.sfile = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with -s flag!");
      }
    } else if (stricmp("-o", argv[i])) {
      if (opt.ofile) {
        ErrorFatal("-o flag used twice!");
      }
      i++;
      if (i < argc) {
        opt.ofile = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with -o flag!");
      }
    } else if (stricmp("-blank", argv[i])) {
      opt.blank=1;
    } else if (stricmp("-b", argv[i])) {
      if (opt.runnable) {
        ErrorFatal("-b flag used twice!");
      }
      opt.runnable=run_blib;
    } else if (stricmp("-depend", argv[i])) {
      if (opt.dfile) {
        ErrorFatal("-depend flag used twice!");
      }
      i++;
      if (i < argc) {
        opt.dfile = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with -depend flag!");
      }
    } else if (strncmp("-I", argv[i], sizeof("-I")-1) == 0) {
      /* Includes must go in in order! */
      arg = &argv[i][2];
      if (*arg) {
        inc = Malloc(sizeof(*inc));
        inc->path = strdup_strip(arg);
        inc->next = NULL;
        *inc_tail = inc;
        inc_tail  = &inc->next;
      } else {
        ErrorFatal("No path passed with -I flag!");
      }
    } else if (strncmp("-D", argv[i], sizeof("-D")-1)==0) { /* case sensitive */
      arg = &argv[i][2];
      if (*arg == 0) {
        i++;
        if (i < argc) {
          arg = argv[i];
        } else {
          ErrorFatal("No definition variable given with -D");
        }
      }
      arg = strdup_strip(arg);
      val = strchr(arg,'=');
      if (val != NULL)
        *val++ = 0;

      define         = Malloc(sizeof(*define));
      define->name   = arg;
      define->value  = val;
      define->invert = 0;
      define->next   = opt.defines;
      opt.defines    = define;
    } else if (strncmp("-U", argv[i], sizeof("-U")-1)==0) { /* case insensitive */
      arg = &argv[i][2];
      if (*arg == 0) {
        i++;
        if (i < argc) {
          arg = argv[i];
        } else {
          ErrorFatal("No (un)definition variable given with -U");
        }
      }
      arg = strdup_strip(arg);
      val = strchr(arg, '=');
      if (val != NULL) {
        ErrorFatal("How can you undefine something to be something? %s", arg);
      }

      define         = Malloc(sizeof(*define));
      define->name   = arg;
      define->value  = NULL;
      define->invert = 1;
      define->next   = opt.defines;
      opt.defines    = define;
    } else if (strcmp("-d", argv[i]) == 0) {
      if (opt.hfile) {
        ErrorFatal("-d flag used twice!");
      }
      i++;
      if (i < argc) {
        opt.hfile = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with -d flag!");
      }
    } else if (strncmp("-z", argv[i], sizeof("-z")-1)==0) { /* case sensitive */
      arg = &argv[i][sizeof("-z")-1];
      if (stricmp("noscl", arg))
	opt.no_scl = 1;
      else if (stricmp("oslib", arg))
        opt.oslib = 1;
      else if (stricmp("base", arg))
        opt.base = 1;
      else if (stricmp("errors", arg))
        opt.mode_errors = 1;
      else
        ErrorFatal("Unknown tweak option -z");
    } else if (strcmp("-32bit", argv[i])==0) { /* case sensitive */
      opt.apcs|= APCS_32BIT;
    } else if (strcmp("-26bit", argv[i])==0) { /* case sensitive */
      opt.apcs&=~APCS_32BIT;
    } else if (strcmp("-apcs", argv[i])==0) { /* case sensitive */
      i++;
      if (i < argc) {
        opt.apcs = apcscli_parse(opt.apcs, argv[i], ErrorFatal);
      } else {
        ErrorFatal("No APCS type supplied!");
      }
    } else {
      if (opt.infile) {
        ErrorFatal("Unknown flag/Only specify one input file: %s", argv[i]);
      }
      opt.infile = strdup_strip(argv[i]);
    }
  }

  /* Doing the help after all options means that any APCS changes made on
     the command line become visible in the help output. */
  if (opt.help_requested) {
    help_text();
    exit(EXIT_FAILURE);
  }
}

void Options_CheckSanity(void) {

  /* CMHG only options should be warned against here */
  if (opt.cmhg) {
    defines_list defines;

    if (opt.sfile)
      CMHGWarning("-s specified");
    if (opt.x_hdr)
      CMHGWarning("-xhdr specified");
    if (opt.x_h)
      CMHGWarning("-xh specified");
    if (opt.oslib)
      CMHGWarning("-zoslib specified");
    if (opt.mode_errors)
      CMHGWarning("-zerrors specified");
    if (opt.base)
      CMHGWarning("-zbase specified");
    if (opt.blank)
      CMHGWarning("-blank specified; generating blank for cmhg");
    if (opt.apcs_used)
      CMHGWarning("-apcs specified");
    if (opt.toolchain_used)
      CMHGWarning("-t<toolchain> specified");
    if (opt.pextended)
      CMHGWarning("-px specified");

    defines = opt.defines;
    while (defines) {
      if (defines->invert) {
        CMHGWarning("-U specified");
        break; /* Only display it once */
      }
      defines=defines->next;
    }
  }

  if (opt.infile == NULL)
  {
    if (opt.blank)
      ErrorFatal("No blank template file specified!");
    else
      ErrorFatal("No input file specified!");
  }
  if (!opt.blank)
  {
    if ((opt.ofile == NULL) && (opt.sfile == NULL) && (opt.hfile == NULL) &&
        (opt.x_hdr == NULL) && (opt.x_h == NULL))
      ErrorFatal("No output file specified!");
  }
  else
  {
    if (! ((opt.ofile == NULL) && (opt.sfile == NULL) && (opt.hfile == NULL)))
      ErrorFatal("Output files specified with blank template!");
  }

  /* We need a temporary file for the assembler */
  if (opt.sfile==NULL) {
    opt.stemp=1;
    opt.sfile=file_temp();
    if (opt.sfile==NULL)
      ErrorFatal("Failed to find a temporary file for assembly");
  }

  /* Toolchain specific checks */
  switch (opt.toolchain)
  {
    case tc_gcc:
      if (opt.dfile)
        Warning("GCC has no support for -depend flag");
      break;

    case tc_lcc:
      if (opt.throwback)
        Warning("LCC has no support for -throwback flag");
      if (opt.dfile)
        Warning("LCC has no support for -depend flag");
      break;

    case tc_norcroft:
      break;
  }
}

void Options_CheckSanityPostRead(void) {

  /* APCS variant checks */
  if (opt.apcs & APCS_REENTRANT)
    ErrorFatal("APCS mode 'reentrant' not supported by CMunge");
#if 0
  if (!(opt.apcs & APCS_SWSTACKCHECK))
    ErrorFatal("APCS mode 'swstackcheck' is required by CMunge");
  if (opt.apcs & APCS_FPREGARGS)
    Warning("APCS mode 'fpregargs' is not recommended for use with CMunge");
#endif

  if (opt.mode_errors)
  {
    /* Different rules are enforced for 'errors' mode, most of which
       are controlled in readfile.c in the command structures. */
  }
  else
  {
    if (!opt.help)
      ErrorFatal("No help-string specified!");
    if (!opt.title)
      ErrorFatal("No title-string specified!");

    if (opt.swi_base == 0) {
      if (opt.swi_handler)
        ErrorFatal("SWI handler defined, but no SWI chunk!");
      if (opt.swi_names)
        ErrorFatal("SWI names defined, but no SWI chunk!");
      if (opt.swi_decoder)
        ErrorFatal("SWI decoder defined, but no SWI chunk!");
    } else {
      if (!opt.swi_handler && !opt.swi_codesupplied)
        ErrorFatal("SWI chunk defined, but no SWI handler!");
      if (!opt.swi_names)
      {
        Warning("No SWI prefix specified; assuming module title");
        opt.swi_names = Malloc(sizeof(*opt.swi_names));
        opt.swi_names->name = strdup_strip(opt.title);
        opt.swi_names->handler = NULL; /* Never actually used */
        opt.swi_names->next = NULL;
      }
    }

    if (opt.pdriver_entry && opt.pdriver_names == NULL)
      Warning("PDriver entry point defined but no handlers");
    
    if (opt.errors && opt.error_base == 0 && !opt.errors_special)
      ErrorFatal("Errors defined, but no error base");
  }
}
