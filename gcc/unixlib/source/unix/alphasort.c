/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/alphasort.c,v $
 * $Date: 2002/02/14 15:56:38 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* unix.c.alphasort.

   Written by Nick Burrett, 3 November 1996.  */

#include <dirent.h>
#include <string.h>

int
alphasort (const struct dirent **a, const struct dirent **b)
{
  return strcmp ((*a)->d_name, (*b)->d_name);
}
