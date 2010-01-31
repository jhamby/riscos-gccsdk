/* getgrouplist ()
 * Copyright (c) 2008 UnixLib Developers
 * Written by Theo Markettos.
 */

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pthread.h>

/* enable this to be built on a non-RISC OS system to test */
#ifndef __riscos
#define __set_errno(x) x
#else
#include <internal/unix.h>
#endif


/* getgrouplist():

Standard:
http://refspecs.freestandards.org/LSB_3.1.0/LSB-Core-generic/LSB-Core-generic/baselib-getgrouplist-3.html
Quote:

Description 

The getgrouplist() function shall fill in the array groups with the
supplementary groups for the user specified by user. On entry, ngroups shall
refer to an integer containing the maximum number of gid_t members in the
groups array. The group group shall also be included. On success, the value
referred to by ngroups shall be updated to contain the number of gid_t
objects copied.

Return Value

On success, if there was sufficient room to copy all the supplementatry
group identifiers to the array identified by groups, getgrouplist() shall
return the number of gid_t objects copied, and the value referenced by
ngroups shall be updated. If there was not sufficient room to copy all the
supplementary group identifiers, getgrouplist() shall return -1, and update the
value referenced by ngroups to the number actually copied.

If user does not refer to a valid user on the system, getgrouplist() shall
return 0, and set the value referenced by ngroups to 0

*/


int
getgrouplist (const char *user, gid_t group, gid_t *groups, int *ngroups)
{
  struct group *grp;
  int grps_copied=0;
  int moretocome=0;

#ifdef __riscos
  PTHREAD_UNSAFE_CANCELLATION
#else
  FILE *f;
#endif  
  /* Need a group name to lookup */
  if (user == NULL)
    return __set_errno (EINVAL);

  /* And somewhere to store the results */
  if (groups == NULL)
    return __set_errno (EINVAL);

  if (ngroups == NULL)
    return __set_errno (EINVAL);

  /* Need space to copy in at least one group ID */
  if (*ngroups <= 0)
    return __set_errno (EINVAL);

#ifndef __riscos
/* If building on a non-RISC OS system, use fgetgrent so we can read a test group file */
  f=fopen("test.group","r");

  if (!f) { fprintf(stderr,"Can't file group file test.group\n");abort();}
#endif

  /* Iterate through the group structure, copying gids of groups
   * we find that match.  Leave at least one space at the end.
   */
#ifdef __riscos
  while ((grp = getgrent ()) !=NULL)
#else
  while (( (grp = fgetgrent(f))!=NULL) )
#endif
    {
      char **mem;

      for (mem = grp->gr_mem;
	   *mem && grps_copied < *ngroups;
           ++mem)
        {
          if ((strcmp (*mem, user) == 0) && (grp->gr_gid!=group))
          {  
            if ((grps_copied+1) < (*ngroups))
              groups[grps_copied++] = grp->gr_gid;
            else
              moretocome++;
          }
        }
    }
  endgrent ();
  
  /* if we failed to find this group anywhere, then indicate that */
  if (grps_copied==0)
  {
    *ngroups=0;
    return 0;
  }
  
  /* tag on to the end the group we were told to add */
  groups[grps_copied++] = group;

  /* Return the number we copied */
  *ngroups = grps_copied;

  /* Return -1 if there are still more to copy */
  if (moretocome)
    return -1;

  /* We succeeded, so return the number of groups we did find */
  return grps_copied;
}

