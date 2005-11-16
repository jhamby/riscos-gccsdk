/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/locale/territory.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <locale.h>

/* Global used for all calls to the Territory module. These variable
   contain the territory number as set by setlocale. A value of
   -1 means use the C locale.  */
int __locale_territory[LC_ALL + 1];
