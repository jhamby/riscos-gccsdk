/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/alphasort,v $
 * $Date: 1997/10/09 20:00:49 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: alphasort,v 1.5 1997/10/09 20:00:49 unixlib Exp $";
#endif

/* unix.c.alphasort.

   Written by Nick Burrett, 3 November 1996.  */

#include <dirent.h>
#include <string.h>

int
alphasort (const ptr_t a, const ptr_t b)
{
  return strcmp (((struct dirent *) a)->d_name,
		 ((struct dirent *) b)->d_name);
}
