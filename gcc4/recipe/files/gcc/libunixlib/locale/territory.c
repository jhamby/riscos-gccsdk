/* __locale_territory
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <locale.h>

/* Global used for all calls to the Territory module. These variable
   contain the territory number as set by setlocale. A value of
   -1 means use the C locale.  */
int __locale_territory[LC_ALL + 1];
