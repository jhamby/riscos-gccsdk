/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/memset.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: memset.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <string.h>
#include <strings.h>

void
bzero (void *s, size_t n)
{
  memset (s, 0, n);
}
