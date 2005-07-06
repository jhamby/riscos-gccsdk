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

#ifndef GCC_BIN_DIR
#define GCC_BIN_DIR ""
#endif

#define OBJASM "objasm"
#define AS GCC_BIN_DIR "/as"

static char buf[2048];

void AssembleFile(void) {

  int rc;

  if (opt.ofile) {
    int throwback;
#ifdef CROSS_COMPILE
    throwback = 0;
#else
    throwback = opt.throwback;
#endif
    switch (opt.toolchain)
    {
      case tc_norcroft:
        {
          unsigned long flags = opt.apcs;
          apcsoptions_t *aarg;
          char *bufend;
          bufend = buf + sprintf(buf, OBJASM " -nowarn %s %s -o %s -apcs 3",
                                 throwback ? "-throwback " : "",
                                 opt.sfile, opt.ofile);

          for (aarg = apcsoptions; aarg->name!=NULL; ++aarg)
          {
            if (aarg->bic & (APCS_32BIT | APCS_SWSTACKCHECK | APCS_FPREGARGS))
            {
              /* Only process flags that objasm is known to accept */
              if ((flags & aarg->bic) == aarg->orr)
                bufend += sprintf(bufend, "/%s", aarg->name);
            }
          }
        }
        break;
      case tc_gcc:
      case tc_lcc:
        sprintf(buf, AS " %s -objasm -module %s %s -o %s",
                          (CODE32) ? "-apcs32 -apcsfpv3 -t ARM6" : "",
                          throwback ? "-throwback " : "",
                          opt.sfile, opt.ofile);
        break;
    }

    /* printf("Command: %s\n",buf); */

    rc = system(buf);
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
