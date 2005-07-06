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

/* Instantiate the global options struct here */
options opt;

void Options_Init(void) {

  opt.runnable    = run_none;
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
  opt.swi_decoder = NULL;
  opt.swi_codesupplied = 0;
  opt.irqs        = NULL;
  opt.events      = NULL;
  opt.init        = NULL;
  opt.final       = NULL;
  opt.lib_init    = NULL;
  opt.generics    = NULL;
  opt.vectors     = NULL;
  opt.vector_traps = NULL; /* vectors which can be post processed */
  opt.ofile       = NULL; /* AOF output */
  opt.sfile       = NULL; /* Assembler output */
  opt.pfile       = NULL; /* Pre-processor output */
  opt.hfile       = NULL; /* Header output */
  opt.throwback   = 0;
  opt.infile      = NULL;
  opt.atline      = 0;
  opt.defines     = NULL;
  opt.version     = 0;
  opt.date        = NULL;
  opt.oslib       = 0;
  opt.mfile       = NULL;
  opt.dfile       = NULL;
  opt.includes    = NULL;
  opt.base        = 0;
  opt.mode_errors = 0; /* We're not in 'errors' mode */
  opt.datestring  = NULL;
  opt.cmhg        = 0; /* We're not CMHG */
  opt.errors      = NULL;
  opt.errors_special = 0;
  /* Export files */
  opt.x_hdr       = NULL;
  opt.x_h         = NULL;
  opt.apcs        = APCS_32BIT |
                    APCS_SWSTACKCHECK |
                    APCS_FP;
  opt.toolchain   = tc_norcroft;
  opt.apcs_used   = 0;
  opt.toolchain_used = 0;
}

/* This is a global variable so that we can use it in the other files */
apcsoptions_t apcsoptions[] = {
  { "reentrant",      5, APCS_REENTRANT,    APCS_REENTRANT,
    "Reentrant APCS variant" },
  { "nonreentrant",   8, APCS_REENTRANT,    0,
    "Non reentrant APCS variant" },
  { "32bit",          2, APCS_32BIT,        APCS_32BIT,
    "32 bit APCS variant" },
  { "26bit",          2, APCS_32BIT,        0,
    "26 bit APCS variant" },
  { "fpe3",           4, APCS_FPE3,         APCS_FPE3,
    "Floating point emulator 3 compatibility" },
  { "fpe2",           4, APCS_FPE3,         0,
    "Floating point emulator 2 compatibility" },
  { "swstackcheck",   4, APCS_SWSTACKCHECK, APCS_SWSTACKCHECK,
    "Software stack checking APCS variant" },
  { "noswstackcheck", 6, APCS_SWSTACKCHECK, 0,
    "No software stack checking APCS variant" },
  { "fpregargs",      3, APCS_FPREGARGS,    APCS_FPREGARGS,
    "FP arguments passed in FP registers" },
  { "nofpregargs",    5, APCS_FPREGARGS,    0,
    "FP arguments are not passed in FP registers" },
  { "fp",             2, APCS_FP,           APCS_FP,
    "Arguments referenced through the frame pointer" },
  { "nofp",           4, APCS_FP,           0,
    "Arguments referenced through the stack pointer" },
  { NULL }
};

/* Parse the -apcs arguments to generate a new set of flags */
static unsigned long parse_apcs(unsigned long flags, const char *arg)
{
  if (*arg == '3') arg++;
  while (*arg)
  {
    if (*arg=='/')
      arg++;
    else
    {
      const char *argend;
      int arglen;
      argend = strchr(arg,'/');
      if (argend)
        arglen = argend-arg;
      else
        arglen = strlen(arg);

      {
        apcsoptions_t *aarg = &apcsoptions[0];
        while (aarg->name!=NULL)
        {
          if ( strncmp(aarg->name, arg, arglen)==0 &&
               ( (arglen == aarg->minabbrev &&
                  arg[aarg->minabbrev]=='\0' || arg[aarg->minabbrev]=='/') ||
                 (strlen(aarg->name) == arglen)
               ) )
            break;
          aarg++;
        }
        /* printf("Option '%.*s' matches '%s'\n",arglen,arg,aarg->name); */
        if (aarg->name==NULL)
          ErrorFatal("APCS flag %s not recognised\n");
        flags &=~ aarg->bic;
        flags |=  aarg->orr;
      }

      arg += arglen;
    }
  }
  return flags;
}

static void help_text(void) {

  format_wrap(stdout,"","",
"\n"
"Make a RISC OS module header for a module written in C/C++.\n"
"\n"
"Usage: cmunge [options] infile\n"
"\n"
"Options:\n"
"\n"
"\t-h            \bOutput this information.\n"
"\t-o <file>     \bName the output AOF file.\n"
"\t-s <file>     \bName the output assembler file.\n"
"\t-d <file>     \bName a C header file that CMunge will generate.\n"
"\t-xhdr <file>  \bName a Assembler SWI header file that CMunge will generate.\n"
"\t-xh <file>    \bName a C SWI header file that CMunge will generate.\n"
"\t-p            \bPre-process the file with cpp before parsing.\n"
/*"-b            \bUse _blib_entermodule instead of SCL version\n" */
"\t-I<directory> \bInclude <directory> on the #include search path.\n"
"\t-D<symbol>=<value>\b\n"
"\t-D<symbol>    \bDefine <symbol> on entry to the preprocessor.\n"
"\t-U<symbol>    \bUndefine <symbol> on entry to the preprocessor.\n"
"\t-depend <file>\bOutput 'make' include file dependency information to <file>.\n"
"\t-throwback    \bUse throwback for error processing.\n"
"\t-zbase        \bDefine symbol Image__RO_Base in generated header.\n"
"\t-zerrors      \bDefine errors and veneers only (for use in applications).\n"
"\t-zoslib       \bUse '#include \"oslib/os.h\"' (OSLib) instead of "
                 "'#include \"kernel.h\"' (Acorn) in generated C header file.\n"
"\t-blank        \bGenerate a blank cmhg file to infile.\n"
"\t-cmhg         \bGive warnings for non CMHG values.\n"
"\t-tgcc         \bUse GCC tool chain to generate output.\n"
"\t-tlcc         \bUse LCC tool chain to generate output.\n"
"\t-tnorcroft    \bUse Norcroft tool chain to generate output (default).\n"
"\t-32bit        \bGenerate 32-bit compatible code (default).\n"
"\t-26bit        \bGenerate 26-bit only code.\n"
"\t-apcs 3/<flags>\bSpecify APCS variant to use.\n",
"");

  /* Now print out the APCS flags */
  {
    int longest=0;
    apcsoptions_t *arg;
    arg = apcsoptions;
    while (arg->name != NULL)
    {
      int len=strlen(arg->name);
      if (len > longest)
        longest = len;
      arg++;
    }
    arg = apcsoptions;

    printf("APCS flags: (*=currently selected)\n");
    while (arg->name != NULL)
    {
      int count;
      int len = strlen(arg->name);
      if (len == arg->minabbrev)
        printf("  /%s   ",arg->name);
      else
        printf("  /%.*s[%s] ",arg->minabbrev, arg->name,
                             &arg->name[arg->minabbrev]);
      for (count=longest-len; count>0; count--)
        putc(' ', stdout);
      printf("%s %s\n",((opt.apcs & arg->bic)==arg->orr) ? "*" : " ",
                       arg->desc);
      arg++;
    }
  }
}

void Options_CL(int argc, char *argv[]) {

  int i;
  defines_list define;
  include_list inc, *inc_tail;
  char *arg;
  char *val;

  inc_tail = &opt.includes;
  for (i = 1; i < argc; i++) {
    if (!stricmp("-throwback", argv[i])) {
      opt.throwback=1;
    } else if (strncmp("-t", argv[i], sizeof("-t")-1)==0) {
      char *arg = &argv[i][2];
      if (!stricmp("norcroft", arg)) {
        opt.toolchain = tc_norcroft;
      } else if (!stricmp("gcc", arg)) {
        opt.toolchain = tc_gcc;
      } else if (!stricmp("lcc", arg)) {
        opt.toolchain = tc_lcc;
      }
      else
        ErrorFatal("Toolchain %s not known!",arg);
      opt.toolchain_used = 1;
    } else if (strncmp("-x", argv[i], sizeof("-x")-1)==0) {
      char *arg = &argv[i][2];
      char **file = NULL; /* Couldn't find opt */
      if (!stricmp("hdr", arg)) {
        file=&opt.x_hdr;
      } else if (!stricmp("h", arg)) {
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
    } else if (!stricmp("-h", argv[i]) || !stricmp("-help", argv[i])) {
      help_text();
      exit(EXIT_FAILURE);
    } else if (!stricmp("-p", argv[i])) {
      if (opt.pfile) {
        ErrorFatal("-p flag used twice!");
      }
      opt.pfile=file_temp();
      if (opt.pfile==NULL)
        ErrorFatal("Failed to find a temporary file for preprocessor");
    } else if (!stricmp("-cmhg", argv[i])) {
      if (opt.cmhg) {
        ErrorFatal("-cmhg flag used twice!");
      }
      opt.cmhg=1;
    } else if (!stricmp("-s", argv[i])) {
      if (opt.sfile) {
        ErrorFatal("-s flag used twice!");
      }
      i++;
      if (i < argc) {
        opt.sfile = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with -s flag!");
      }
    } else if (!stricmp("-o", argv[i])) {
      if (opt.ofile) {
        ErrorFatal("-o flag used twice!");
      }
      i++;
      if (i < argc) {
        opt.ofile = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with -o flag!");
      }
    } else if (!stricmp("-blank", argv[i])) {
      opt.blank=1;
    } else if (!stricmp("-b", argv[i])) {
      if (opt.runnable) {
        ErrorFatal("-b flag used twice!");
      }
      opt.runnable=run_blib;
    } else if (!stricmp("-depend", argv[i])) {
      if (opt.dfile) {
        ErrorFatal("-depend flag used twice!");
      }
      i++;
      if (i < argc) {
        opt.dfile = strdup_strip(argv[i]);
      } else {
        ErrorFatal("No filename passed with -depend flag!");
      }
    } else if (strncmp("-I", argv[i], 2) == 0) {
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
    } else if (strncmp("-D", argv[i], 2)==0) { /* case sensitive */
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
    } else if (strncmp("-U", argv[i], 2)==0) { /* case insensitive */
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
      if (!stricmp("oslib", arg)) {
        opt.oslib = 1;
      } else if (!stricmp("base", arg)) {
        opt.base = 1;
      } else if (!stricmp("errors", arg)) {
        opt.mode_errors = 1;
      } else {
        ErrorFatal("Unknown tweak option -z");
      }
    } else if (strcmp("-32bit", argv[i])==0) { /* case sensitive */
      opt.apcs|= APCS_32BIT;
    } else if (strcmp("-26bit", argv[i])==0) { /* case sensitive */
      opt.apcs&=~APCS_32BIT;
    } else if (strcmp("-apcs", argv[i])==0) { /* case sensitive */
      i++;
      if (i < argc) {
        opt.apcs = parse_apcs(opt.apcs, argv[i]);
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
    opt.sfile=file_temp();
    if (opt.sfile==NULL)
      ErrorFatal("Failed to find a temporary file for assembly");
  }
}

void Options_CheckSanityPostRead(void) {

  /* APCS variant checks */
#if 0
  if (opt.apcs & APCS_REENTRANT)
    ErrorFatal("APCS mode 'reentrant' not supported by CMunge");
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
    }

    if (opt.errors && opt.error_base == 0 && !opt.errors_special)
      ErrorFatal("Errors defined, but no error base");
  }
}
