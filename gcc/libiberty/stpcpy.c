/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stpcpy.c,v $
 * $Date: 2000/11/08 09:38:59 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stpcpy.c,v 1.1.1.1 2000/11/08 09:38:59 admin Exp $";
#endif

#include <string.h>

char *
stpcpy (char *s, const char *s2)
{
  while ((*s++ = *s2++))
    ;

  return s - 1;
}

