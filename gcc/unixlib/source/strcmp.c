/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strcmp.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcmp.c,v 1.1.1.1 2000/07/15 14:52:09 nick Exp $";
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
