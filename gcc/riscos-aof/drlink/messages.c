/*
** Drlink AOF Linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This module contains the procedures called to produce error
** messages. Under RISCOS, they can be sent to either the screen
** or a 'Throwback' window
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "drlhdr.h"
#include "filehdr.h"
#include "procdefs.h"

#ifdef TARGET_RISCOS
#include <kernel.h>
#endif

/* Private declarations */

#ifdef TARGET_RISCOS

static int last_level;
static bool first_error;

#define DDEUtils_ThrowbackStart 0x42587
#define DDEUtils_ThrowbackSend 0x42588
#define DDEUtils_ThrowbackEnd 0x42589

#define Throwback_ReasonProcessing 0
#define Throwback_ReasonErrorDetails 1

/* Throwback error levels */

#define  WARNING 0
#define  ERROR 1
#define  FATAL 2

#define  DUMMYLINE 1

/*
** 'start_throwback' is called to start a throwback session
*/
void start_throwback(void) {
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  swierror = _kernel_swi(DDEUtils_ThrowbackStart, &regs, &regs);
  if (swierror!=NIL) {
    swierror = _kernel_last_oserror();	/* Loose SWI error just logged */
    opt_throw = FALSE;
    error("Warning: 'Throwback' is not available. Option ignored");
  }
  first_error = TRUE;
  last_level = 0;
}

/*
** 'end_throwback' is called to end a throwback session
*/
void end_throwback(void) {
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  opt_throw = FALSE;
  swierror = _kernel_swi(DDEUtils_ThrowbackEnd, &regs, &regs);
  if (swierror!=NIL) {
    swierror = _kernel_last_oserror();	/* Loose SWI error just logged */
    error("Error: Error occured trying to end 'throwback' session: %s", &swierror->errmess);
  }
}

/*
** 'throwback_message' is called to display an error message in a
** 'throwback' window
*/
static void throwback_message(char *text) {
  int errlevel;
  char *filename;
  _kernel_oserror *swierror;
  _kernel_swi_regs regs;
  filename = imagename;
  if (filename==NIL) filename = "!RunImage";
  if (first_error) {	/* First error - Register file with Throwback */
    first_error = FALSE;
    regs.r[0] = Throwback_ReasonProcessing;
    regs.r[2] = COERCE(filename, int);
    swierror = _kernel_swi(DDEUtils_ThrowbackSend, &regs, &regs);
    if (swierror!=NIL) {
      opt_throw = FALSE;
      error("Error: Error occured sending 'throwback' message: %s", &swierror->errmess);
      swierror = _kernel_last_oserror();	/* Loose SWI error just logged */
      return;
    }
  }
  switch (*text) {	/* Decide on message type according to first char of error */
  case ' ':
    errlevel = last_level;
    break;
  case 'E':
    errlevel = ERROR;
    break;
  case 'F':
    errlevel = FATAL;
    break;
  default:
    errlevel = WARNING;
  }
  last_level = errlevel;
  regs.r[0] = Throwback_ReasonErrorDetails;
  regs.r[2] = COERCE(filename, int);
  regs.r[3] = DUMMYLINE;
  regs.r[4] = errlevel;
  regs.r[5] = COERCE(text, int);
  swierror = _kernel_swi(DDEUtils_ThrowbackSend, &regs, &regs);
  if (swierror!=NIL) {
    opt_throw = FALSE;
    error("Error: Error occured sending 'throwback' message: %s", &swierror->errmess);
    swierror = _kernel_last_oserror();	/* Loose SWI error just logged */
    printf(text);
  }
}

#endif

/*
** 'error' is called when a linker error has been detected to print
** an error message. The vast majority of messages ar printed this
** way. Many information messages are simply "printf'ed", but all
** warning, error and fatal messages are handled by 'error'. The
** severity of the error is determined buy looking at the first
** character of the message, that is, whether it is 'W' (warning),
** 'E' (error) or 'F' (fatal). Anything else is an informational
** message (they usually start with 'D' or a blank). If
** the 'throwback' option is used, error messages are sent to a
** 'throwback' window otherwise they are just printed.
*/
void error(char *msg, ...) {
  char *p1, *p2, *p3, *p4;
  va_list parms;
  va_start(parms, msg);
  p1 = va_arg(parms, char *);
  p2 = va_arg(parms, char *);
  p3 = va_arg(parms, char *);
  p4 = va_arg(parms, char *);
  va_end(parms);
#ifdef TARGET_RISCOS
  if (opt_throw && (*msg==' ' || *msg=='W' || *msg=='E' ||  *msg=='F')) {
    char text[MSGBUFLEN];
    sprintf(text, msg, p1, p2, p3, p4);
    throwback_message(text);
  }
  else {
    printf(msg, p1, p2, p3, p4);
    printf("\n");
  }
#else
  printf(msg, p1, p2, p3, p4);
  printf("\n");
#endif
  switch (*msg) {
  case 'W':
    warnings+=1;
    break;
  case 'E':
    errors+=1;
    break;
  case 'F':
    tidy_files();
    release_heap();
#ifdef TARGET_RISCOS
    if (opt_throw) end_throwback();
#endif
    exit(EXIT_FATAL);
  }
}

/*
** 'got_errors' is called to see if any errors have occured during the link
*/
bool got_errors(void) {
  return errors>0;
}

/*
** 'announce' is called to say which version of the linker is in use
*/
void announce(void) {
  error("Drlink AOF Linker  Version %s", VERSION);
}

