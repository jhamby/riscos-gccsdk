/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strncmp.c,v $
 * $Date: 2002/09/24 21:02:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strncmp.c,v 1.3 2002/09/24 21:02:36 admin Exp $";
#endif

#include <string.h>

int
strncmp (const char *s1, const char *s2, size_t n)
{
  int i;

  if (!n)
    return 0;

  while ((i = *s1) && i == *s2 && --n)
    s1++, s2++;

  return (*(const unsigned char *) s1 - *(const unsigned char *) s2);
}
