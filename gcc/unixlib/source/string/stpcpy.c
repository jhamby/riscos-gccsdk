/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stpcpy.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stpcpy.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <string.h>

char *
stpcpy (char *s, const char *s2)
{
  while ((*s++ = *s2++))
    ;

  return s - 1;
}

