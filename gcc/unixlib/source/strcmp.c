/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strcmp,v $
 * $Date: 1997/10/09 19:59:40 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcmp,v 1.5 1997/10/09 19:59:40 unixlib Exp $";
#endif

#include <string.h>

int
strcmp (register const char *s1, register const char *s2)

{
  register int i;

  while ((i = *s1) && i == *s2)
    s1++, s2++;

  return (*(unsigned char *) s1 - *(unsigned char *) s2);
}
