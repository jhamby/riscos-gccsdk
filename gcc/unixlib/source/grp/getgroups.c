/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

/* Supplementary group reading and setting functions */

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pthread.h>

#define MAX_GROUPS 10 /* Maximum number of groups initgroups() can handle */

static gid_t *g_gidset = NULL; /* List of group IDs the this user is in */
static int g_ngroups=0; /* number of items in the list */


/* Get a list of all supplementary groups this user is in */
int
getgroups (int gidsetlen, gid_t *gidset)
{
  PTHREAD_UNSAFE

  if (gidsetlen == 0)
    return g_ngroups;

  if (gidset == NULL)
    return __set_errno (EINVAL);

  if (gidsetlen > g_ngroups)
    gidsetlen = g_ngroups;

  if (gidsetlen > 0) memcpy (gidset, g_gidset, gidsetlen * sizeof (gid_t));
  return gidsetlen;
}


/* Set the list of supplementary groups this user is in */
int
setgroups (int ngroups, const gid_t *gidset)
{
  PTHREAD_UNSAFE

  if (g_gidset) free (g_gidset);
  g_gidset = NULL;

  g_ngroups = ngroups;
  if (ngroups > 0)
    {
      g_gidset = malloc (ngroups * sizeof (gid_t));
      if (g_gidset == NULL)
        return __set_errno (ENOMEM);
      memcpy (g_gidset, gidset, ngroups * sizeof (gid_t));
    }
  return 0;
}


/* Build a list of all groups the user is in, then call setgroups on the list*/
int
initgroups (const char *name, gid_t basegid)
{
  struct group *grp;
  int ngroups=0;
  gid_t gidset[MAX_GROUPS];

  PTHREAD_UNSAFE

  while ( (grp = getgrent ()) !=NULL)
    {
      char **mem = grp->gr_mem;

      while (*mem && ngroups < MAX_GROUPS)
        {
          if (strcmp (*mem, name) == 0)
            gidset[ngroups++] = grp->gr_gid;
          mem++;
        }

    }
  endgrent ();
  return setgroups (ngroups, gidset);
}
