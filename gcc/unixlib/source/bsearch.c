/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/bsearch.c,v $
 * $Date: 2001/09/04 16:32:03 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: bsearch.c,v 1.2.2.2 2001/09/04 16:32:03 admin Exp $";
#endif

#include <unixlib/unix.h>
#include <stddef.h>
#include <stdlib.h>

void *
bsearch (const void *key, const void *base_, size_t nmemb,
	 size_t size, int (*compar) (const void *, const void *))

{
  char *base, *member;
  int comparison;

  base = (char *) base_;

  while (nmemb)
    {
      member = base + ((nmemb >> 1) * size);
      comparison = __funcall ((*compar), (key, member));
      if (comparison == 0)
	return member;
      if (comparison > 0)
        {
	  base = member + size;
	  nmemb = nmemb - (nmemb >> 1) - 1;
	}
      else
	nmemb = (nmemb >> 1);
    }

  return NULL;
}
