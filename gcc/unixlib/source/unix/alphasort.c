/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/alphasort.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: alphasort.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
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
