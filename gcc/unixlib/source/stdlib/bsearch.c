/* bsearch ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
