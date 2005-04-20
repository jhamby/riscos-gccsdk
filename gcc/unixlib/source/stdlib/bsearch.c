/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/bsearch.c,v $
 * $Date: 2005/04/13 19:20:05 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: bsearch.c,v 1.4 2005/04/13 19:20:05 nick Exp $";
#endif

#include <unixlib/unix.h>
#include <stddef.h>
#include <stdlib.h>

void *
bsearch (const void *key, const void *base, size_t nmemb,
	 size_t size, int (*compar) (const void *, const void *))

{
  char *member;
  int comparison;

  while (nmemb)
    {
      member = (char *) ((size_t) base + ((nmemb >> 1) * size));
      comparison = __funcall ((*compar), (key, member));
      if (comparison == 0)
	return (void *) member;
      if (comparison > 0)
        {
	  base = (const void *) ((size_t) member + size);
	  nmemb = nmemb - (nmemb >> 1) - 1;
	}
      else
	nmemb = (nmemb >> 1);
    }

  return NULL;
}
