/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strcpy,v $
 * $Date: 1999/02/07 20:52:38 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strcpy,v 1.6 1999/02/07 20:52:38 unixlib Exp $";
#endif

#include <string.h>

char *
strcpy (char *s, const char *s2)
{
  char *s1 = s;

  while ((*s1++ = *s2++))
    ;

  return s;
}
