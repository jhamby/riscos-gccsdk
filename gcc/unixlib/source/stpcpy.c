/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/stpcpy,v $
 * $Date: 1999/02/07 20:52:36 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stpcpy,v 1.6 1999/02/07 20:52:36 unixlib Exp $";
#endif

#include <string.h>

char *
stpcpy (char *s, const char *s2)
{
  while ((*s++ = *s2++))
    ;

  return s - 1;
}

