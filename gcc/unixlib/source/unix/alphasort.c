/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/alphasort.c,v $
 * $Date: 2001/09/14 14:01:17 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: alphasort.c,v 1.2.2.1 2001/09/14 14:01:17 admin Exp $";
#endif

/* unix.c.alphasort.

   Written by Nick Burrett, 3 November 1996.  */

#include <dirent.h>
#include <string.h>

int
alphasort (const struct dirent **a, const struct dirent **b)
{
  return strcmp ((*a)->d_name, (*b)->d_name);
}
