/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/memset,v $
 * $Date: 1997/10/09 19:59:39 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memset,v 1.6 1997/10/09 19:59:39 unixlib Exp $";
#endif

#include <string.h>

void
bzero (void *s, size_t n)
{
  memset (s, 0, n);
}
