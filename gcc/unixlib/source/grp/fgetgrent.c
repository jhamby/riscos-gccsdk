/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/Attic/fgetgrent.c,v $
 * $Date: 2001/09/11 13:32:33 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetgrent.c,v 1.1.2.1 2001/09/11 13:32:33 admin Exp $";
#endif

/* Read a group password file entry. */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>

/* Read one entry from the given stream.  */
struct group *
fgetgrent (FILE * stream)
{
  static struct group grp;

  if (stream == NULL)
    return NULL;

  return __grpread (stream, &grp);
}
