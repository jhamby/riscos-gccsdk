/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strncmp,v $
 * $Date: 1997/10/09 19:59:41 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strncmp,v 1.2 1997/10/09 19:59:41 unixlib Exp $";
#endif

#include <string.h>

int
strncmp (register const char *s1, register const char *s2, register size_t n)
{
  register int i;

  if (!n)
    return 0;

  while ((i = *s1) && i == *s2 && --n)
    s1++, s2++;

  return (*(unsigned char *) s1 - *(unsigned char *) s2);
}
