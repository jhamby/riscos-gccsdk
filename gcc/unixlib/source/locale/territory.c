/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/locale/c/territory,v $
 * $Date: 1997/10/08 12:48:11 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: territory,v 1.1 1997/10/08 12:48:11 unixlib Exp $";
#endif

#include <locale.h>

/* Global used for all calls to the Territory module. These variable
   contain the territory number as set by setlocale. A value of
   -1 means use the C locale.  */
int __locale_territory[LC_ALL + 1];
