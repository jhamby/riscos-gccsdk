/*
 * Copyright (C) 1999-2003 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "datestamp.h"
#include "error.h"
#include "mem.h"
#include "options.h"
#include "system.h"
#include "apcscli.h"

static char buf[2048];

void AssembleFile(void) {

  int rc;

  if (opt.ofile) {
    int throwback;
#ifdef __riscos
    throwback = opt.throwback;
#  define GCC_BINARY_NAME "gcc"
#else
    throwback = 0;
#  define GCC_BINARY_NAME "arm-unknown-riscos-gcc"
#endif

    switch (opt.toolchain)
    {
      case tc_norcroft:
        {
          unsigned long flags = opt.apcs;
          char *bufend;
          bufend = buf + sprintf(buf, "objasm -nowarn %s %s -o %s -apcs ",
                                 throwback ? "-throwback " : "",
                                 opt.sfile, opt.ofile);
          bufend += apcscli_buildstring(flags, ~(APCS_32BIT |
                                                 APCS_SWSTACKCHECK),
                                        1, bufend, buf+2048-bufend);
        }
        break;
      case tc_gcc:
      case tc_lcc:
        sprintf(buf, GCC_BINARY_NAME " -xassembler -mmodule %s -c %s -o %s",
                          throwback ? "-mthrowback" : "",
                          opt.sfile, opt.ofile);
        break;
    }

    /* printf("Command: %s\n",buf); */

    rc = our_system(buf);
    switch (rc) {
      case EXIT_SUCCESS:
        /* Yo, success! print a message ? Nah, just return */
        return;

      case -2:
        /* Erm... not enough memory... or not there... or something... */
        ErrorFatal("Unable to spawn assembler");
        break;

      default:
        /* They returned something icky. Obviously that's a fault; they
           should have already displayed an error as to the fault though.
         */
        fprintf(stderr,"Assembling: %s\n",buf);
        fprintf(stderr,"Assembler returned %i unexpectedly; aborting\n",rc);
        exit(rc);
    }
  }
}
