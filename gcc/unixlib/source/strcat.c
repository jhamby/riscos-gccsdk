/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strcat,v $
 * $Date: 1999/02/07 20:52:37 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcat,v 1.6 1999/02/07 20:52:37 unixlib Exp $";
#endif

#include <string.h>

char *
strcat (char *s, const char *s2)
{
  char *s1 = s;

  while (*s1++)
    ;
  --s1;

  while ((*s1++ = *s2++))
    ;

  return s;
}
