/*
** Drlink AOF Linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** This module contains the procedures called to produce error
** messages. Under RISC OS, they can be sent to either the screen
** or a 'Throwback' window
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "drlhdr.h"
#include "filehdr.h"
#include "procdefs.h"

#ifndef CROSS_COMPILE
# include <kernel.h>
#endif

/* Private declarations */

#ifndef CROSS_COMPILE

static int last_level = 0;	/* Last throwback error message level */
static bool first_error = TRUE;	/* TRUE before first time Throwback is used */

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
 * 'start_throwback' is called the first time an error is to be
 * reported via 'throwback' to start the throwback session. It
 * returns TRUE if this worked otherwise it returns FALSE
 */
static bool
start_throwback (const char *filename)
{
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  swierror = _kernel_swi (DDEUtils_ThrowbackStart, &regs, &regs);
  if (swierror != NULL)
    {
      swierror = _kernel_last_oserror ();	/* Lose SWI error just logged */
      opt_throw = FALSE;
      error ("Warning: 'Throwback' is not available. Option ignored");
      return FALSE;
    }
/* Now register file */
  regs.r[0] = Throwback_ReasonProcessing;
  regs.r[2] = COERCE (filename, int);
  swierror = _kernel_swi (DDEUtils_ThrowbackSend, &regs, &regs);
  if (swierror != NULL)
    {
      opt_throw = FALSE;
      error ("Error: Error occured sending 'throwback' message: %s",
	     &swierror->errmess);
      swierror = _kernel_last_oserror ();	/* Lose SWI error just logged */
      return FALSE;
    }
  return TRUE;
}

/*
** 'end_throwback' is called to end a throwback session
*/
void
end_throwback (void)
{
  _kernel_swi_regs regs;
  _kernel_oserror *swierror;
  opt_throw = FALSE;
  swierror = _kernel_swi (DDEUtils_ThrowbackEnd, &regs, &regs);
  if (swierror != NULL)
    {
      swierror = _kernel_last_oserror ();	/* Lose SWI error just logged */
      error ("Error: Error occured trying to end 'throwback' session: %s",
	     &swierror->errmess);
    }
}

/*
** 'throwback_message' is called to display an error message in a
** 'throwback' window
*/
static void
throwback_message (char *text)
{
  int errlevel;
  const char *filename;
  _kernel_oserror *swierror;
  _kernel_swi_regs regs;
  filename = imagename;
  if (filename == NULL)
    filename = "!RunImage";
  if (first_error)
    {
      first_error = FALSE;
      if (!start_throwback (filename))
	return;
    }
  switch (*text)
    {				/* Decide on message type according to first char of error */
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
  regs.r[2] = COERCE (filename, int);
  regs.r[3] = DUMMYLINE;
  regs.r[4] = errlevel;
  regs.r[5] = COERCE (text, int);
  swierror = _kernel_swi (DDEUtils_ThrowbackSend, &regs, &regs);
  if (swierror != NULL)
    {
      opt_throw = FALSE;
      error ("Error: Error occured sending 'throwback' message: %s",
	     &swierror->errmess);
      swierror = _kernel_last_oserror ();	/* Lose SWI error just logged */
      printf (text);
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
void
error (const char *msg, ...)
{
  char *p1, *p2, *p3, *p4;
  va_list parms;
  va_start (parms, msg);
  p1 = va_arg (parms, char *);
  p2 = va_arg (parms, char *);
  p3 = va_arg (parms, char *);
  p4 = va_arg (parms, char *);
  va_end (parms);
#ifndef CROSS_COMPILE
  if (opt_throw && (*msg == ' ' || *msg == 'W' || *msg == 'E' || *msg == 'F'))
    {
      char text[MSGBUFLEN];
      sprintf (text, msg, p1, p2, p3, p4);
      throwback_message (text);
    }
  else
    {
      printf (msg, p1, p2, p3, p4);
      fputc ('\n', stdout);
    }
#else
  printf (msg, p1, p2, p3, p4);
  fputc ('\n', stdout);
#endif
  switch (*msg)
    {
    case 'W':
      warnings += 1;
      if (!opt_warningerrors)
        break;
    case 'E':
      errors += 1;
      break;
    case 'F':
      tidy_files ();
      release_heap ();
#ifndef CROSS_COMPILE
      if (opt_throw)
	end_throwback ();
#endif
      exit (EXIT_FATAL);
    }
}

/*
** 'got_errors' is called to see if any errors have occured during the link
*/
bool
got_errors (void)
{
  return errors > 0;
}

/*
** 'announce' is called to say which version of the linker is in use
*/
void
announce (void)
{
  error ("Drlink AOF Linker " DL_VERSION);
}
