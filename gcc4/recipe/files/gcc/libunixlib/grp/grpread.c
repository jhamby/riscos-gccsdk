/* __grpread ()
 * Internal group password-file reading functions.
 *
 * Copyright (c) 2002-2006 UnixLib Developers
 */

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>

#define MAX_MEM 10 /*Maximum number of members per group*/


static char *
g_gstrcp (char **lp, char sep)
{
  char *l = *lp, *r = l;

  if (sep == ',' && *l == '\0')
    return NULL;

  while ((*l != sep) && *l)
    l++;
  if (*l)
    *l++ = 0;
  *lp = l;
  return r;
}


static void
g_gdecode (char *line, struct group *grp)
{
  char *lp;
  int i=0;
  static char *mem[MAX_MEM];

  if (!line || !grp)
    return;

  lp = line;

  grp->gr_name = g_gstrcp (&lp, ':');
  grp->gr_passwd = g_gstrcp (&lp, ':');
  grp->gr_gid = atoi (g_gstrcp (&lp, ':'));
  grp->gr_mem = mem;
  do
    mem[i++] = g_gstrcp (&lp, ',');
  while (i < MAX_MEM && mem[i-1]);
  mem[MAX_MEM-1] = NULL;
}

/* Read one entry from the given stream.  */
struct group *
__grpread (FILE * stream, struct group *grp, char *buf, size_t buflen)
{
  char *bp;

  if (stream == NULL)
    return 0;

  /* Get a line, skipping past comment lines.  */
  do
    if (fgets (buf, buflen - 1, stream) == 0)
      return 0;
  while (buf[0] == '#');

  /* Take the line and convert the newline into a zero terminated
     string.  */
  bp = buf;
  while (*bp)
    bp++;
  if (*--bp != '\n')
    return 0;
  *bp = 0;

  /* Decode the line.  */
  g_gdecode (buf, grp);
  return grp;
}
