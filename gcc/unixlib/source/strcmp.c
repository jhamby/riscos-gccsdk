/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strcmp.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcmp.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
#endif

#include <string.h>

int
strcmp (const char *s1, const char *s2)

{
  register int i;

  while ((i = *s1) && i == *s2)
    s1++, s2++;

  return (*(const unsigned char *) s1 - *(const unsigned char *) s2);
}
