/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/bsearch.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: bsearch.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <sys/syslib.h>
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
