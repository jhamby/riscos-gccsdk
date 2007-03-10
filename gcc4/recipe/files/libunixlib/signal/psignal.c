/* psignal ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
