/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memset.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memset.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <string.h>

void
bzero (void *s, size_t n)
{
  memset (s, 0, n);
}
