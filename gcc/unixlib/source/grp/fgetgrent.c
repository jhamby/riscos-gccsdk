/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/fgetgrent.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* Read a group password file entry. */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>
#include <pthread.h>

/* Read one entry from the given stream.
   Not thread safe.  */
struct group *
fgetgrent (FILE * stream)
{
  static struct group grp;
  static char buffer[256];

  PTHREAD_UNSAFE_CANCELLATION

  if (stream == NULL)
    return NULL;

  return __grpread (stream, &grp, buffer, sizeof (buffer));
}
