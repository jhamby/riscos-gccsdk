/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/bsearch,v $
 * $Date: 1997/10/19 19:19:41 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: bsearch,v 1.5 1997/10/19 19:19:41 unixlib Exp $";
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
