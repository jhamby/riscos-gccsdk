/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/psignal.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: psignal.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <stdio.h>
#include <signal.h>
#include <string.h>

/* Print out on stderr a line consisting of the test in S, a colon, a space,
   a message describing the meaning of the signal number SIG and a newline.
   If S is NULL or "", the colon and space are omitted.  */

void
psignal (int sig, const char *s)
{
  const char *colon;

  if (s == NULL || s == '\0')
    s = colon = "";
  else
    colon = ": ";

  if (sig >= 0 && sig < NSIG)
    fprintf (stderr, "%s%s%s\n", s, colon, sys_siglist[sig]);
  else
    fprintf (stderr, "%s%sUnknown signal %d\n", s, colon, sig);
}
