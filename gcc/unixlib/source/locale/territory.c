/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/territory.c,v $
 * $Date: 2000/07/15 14:52:20 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: territory.c,v 1.1.1.1 2000/07/15 14:52:20 nick Exp $";
#endif

#include <locale.h>

/* Global used for all calls to the Territory module. These variable
   contain the territory number as set by setlocale. A value of
   -1 means use the C locale.  */
int __locale_territory[LC_ALL + 1];
