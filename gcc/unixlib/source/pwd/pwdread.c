/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/pwdread.c,v $
 * $Date: 2001/09/11 13:32:33 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: pwdread.c,v 1.2.2.1 2001/09/11 13:32:33 admin Exp $";
#endif

/* pwd.c.pwdread. Internal password-file reading functions.

   Modified from the original c.getpw by Nick Burrett, 13 October 1996.  */

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>

/* p_pstrcp() */

static char *
p_pstrcp (char **lp)
{
  register char *l = *lp, *r = l;

  while ((*l != ':') && *l)
    l++;
  if (*l)
    *l++ = 0;
  *lp = l;
  return (r);
}

/* p_pdecode() */

static void
p_pdecode (char *line, struct passwd *passwd)
{
  char *lp;

  if (! line || ! passwd)
    return;

  lp = line;

  passwd->pw_name = p_pstrcp (&lp);
  passwd->pw_passwd = p_pstrcp (&lp);
  passwd->pw_uid = atoi (p_pstrcp (&lp));
  passwd->pw_gid = atoi (p_pstrcp (&lp));
  passwd->pw_gecos = p_pstrcp (&lp);
  passwd->pw_dir = p_pstrcp (&lp);
  passwd->pw_shell = p_pstrcp (&lp);
}

/* Read one entry from the given stream.  */
struct passwd *
__pwdread (FILE * stream, struct passwd *ppwd)
{
  static char buf[256];
  char *bp;

  if (stream == NULL)
    return NULL;

  /* Get a line, skipping past comment lines.  */
  do
    if (fgets (buf, sizeof (buf) - 1, stream) == 0)
      /* if (getline (buf, sizeof (buf) - 1, stream) == -1) */
      return 0;
  while (buf[0] == '#');

  /* Take the line and convert the newline into a zero terminated
     string.  */
  bp = buf;
  while (*bp)
    bp++;
  if (*--bp != '\n')
    return NULL;
  *bp = 0;

  /* Decode the line.  */
  p_pdecode (buf, ppwd);
  return ppwd;
}
