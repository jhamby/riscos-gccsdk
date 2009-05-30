/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

#include <stdio.h>

#ifdef MEMCHECK_MemCheck
#include "MemCheck.h"
#endif

#include "gfile.h"
#include "options.h"
#include "readfile.h"
#include "writefile.h"
#include "writeheader.h"
#include "writeexport.h"
#include "assemble.h"
#include "datestamp.h"
#include "blank.h"
#include "format.h"

#include "copyright.h"

#include "VersionNum"

int main(int argc, char *argv[]) {

#ifdef MEMCHECK_MemCheck
  MemCheck_Init();
  MemCheck_RegisterArgs(argc, argv);
#endif

  file_init(); /* Initialise file tidy up library */

  printf("CMunge " Module_FullVersionAndDate "\n" Copyright_CMunge "\n");
  /* If you alter this code, insert a printf here to say that you have */
#if 0
  format_wrap(stdout,"","",
                     "The following modifications made by Justin Fletcher :\n"
                     "\t*\bAddition of error-chunk-base-number\n"
                     "\t*\bAssembler header file\n"
                     "\t*\bTidied SWI numbers in header\n"
                     "\t*\bAdded support for -d with no output\n"
                     "\t*\bRe-added throwback support in error.c\n"
                     "\t*\bReworked field parser to be more generic\n"
                     "\t*\bFixed include handling to work with empty files\n"
                     "\t*\bTidied up (?) header file production\n"
                     "\t*\bAdded -blank generation\n"
                     "\t*\bError messages included in header\n"
                     "\t*\bSWI handler extensions\n"
                     "\t*\bFixed -xhdr output to work without output file\n"
                     "\t*\bCorrected throwback handling\n"
                     "\t*\bAdded code handler routines\n"
                     "\t*\bRemoved 'strint' closing parentheses check\n"
                     "\t*\bAdded support for -b for obsolete CMHG feature\n"
                     "\t*\bmodule-is-not-re-entrant now works\n"
                     "\t*\bFixed rogue files being left on failure\n"
                     "\t*\bLong line (objasm) fixes\n"
                     "\t*\bCommand line parser updated to cope with rogue "
                         "commas\n"
                     "\t*\bSWI chunk base must not have x-bit set\n"
                     "\t*\berror-chunk-base-number can have any alignment\n"
                     "\t*\bAdded vector-traps support\n"
                     "\t*\bGeneric-veneers can have private-word registers\n"
                     "\t*\bHeader files are now a little more neatly "
                         "generated\n"
                     "\t*\bSupport for experimental 32-bit header generation\n"
                     "\t*\bSupport for GCC and LCC toolchains (tentative)\n"
                     "\t*\bSupport for building under linux\n"
                     "\t*\bAbstracted APCS options for use in other tools\n",
                     "\n");
  /* ... and writeheader is structured quite a bit differently internally */
  format_wrap(stdout,"","",
                     "The following modifications made by John Tytgat :\n"
                     "\t*\bFixes for OSLib includes\n"
                     "\t*\bSupport for building using 32-bit compatible tools\n",
                     "");
  format_wrap(stdout,"","",
                     "The following modifications were made by GCCSDK Developers :\n"
                     "\t*\bImproved support for GCC toolchain\n",
                     "");
#endif
  Options_Init();
  Options_CL(argc, argv);
  Options_CheckSanity();
  WriteBlank();
  ReadFile();
  Options_CheckSanityPostRead();
  DateStamp();
  WriteFile();
  WriteHeader();
  WriteExport_Hdr();
  WriteExport_H();
  AssembleFile();

  file_final(); /* Mark us as successful */

  return 0;
}
