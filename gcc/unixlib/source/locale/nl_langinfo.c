/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/nl_langinfo.c,v $
 * $Date: 2005/01/09 23:29:04 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
      value = "ISO-8859-1";
      break;

    default:
      value = "";
  }

  return (char *)value;
}

