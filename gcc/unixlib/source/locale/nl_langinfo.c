/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/localeconv.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: localeconv.c,v 1.5 2003/01/21 17:54:22 admin Exp $";
#endif

#include <locale.h>
#include <nl_types.h>
#include <langinfo.h>

char *
nl_langinfo(nl_item item)
{
  const char *value;

  switch (item)
  {
    case CODESET:
      value = "8859-1";
      break;

    default:
      value = "";
  }

  return (char *)value;
}

