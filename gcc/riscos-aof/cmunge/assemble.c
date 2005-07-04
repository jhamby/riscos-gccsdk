/*
 * Copyright (C) 1999-2003 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "kernel.h"

#include "datestamp.h"
#include "error.h"
#include "mem.h"
#include "options.h"

static char buf[2048];

void AssembleFile(void) {

  int rc;

  if (opt.ofile) {
    switch (opt.toolchain)
    {
      case tc_norcroft:
        {
          unsigned long flags = opt.apcs;
          apcsoptions_t *aarg;
          char *bufend;
          bufend = buf + sprintf(buf, "objasm -nowarn %s %s -o %s -apcs 3",
                                 opt.throwback ? "-throwback " : "",
                                 opt.sfile, opt.ofile);

          aarg = apcsoptions;
          while (aarg->name!=NULL)
          {
            if (aarg->bic & (APCS_32BIT | APCS_SWSTACKCHECK | APCS_FPREGARGS))
            {
              /* Only process flags that objasm is known to accept */
              if ((flags & aarg->bic) == aarg->orr)
                bufend += sprintf(bufend, "/%s", aarg->name);
            }
            aarg++;
          }
        }
        break;
      case tc_gcc:
      case tc_lcc:
        sprintf(buf, "as %s -objasm -gcc %s %s -o %s",
                          (CODE32) ? "-apcs32 -apcsfpv3" :
                                                        "",
                          opt.throwback ? "-throwback " : "",
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
        fprintf(stderr,"Assembler returned %i unexpectedly; aborting\n",rc);
        exit(rc);
    }
  }
}
