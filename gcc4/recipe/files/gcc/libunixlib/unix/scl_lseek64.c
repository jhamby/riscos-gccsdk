/* lseek (), lseek64 () for SCL
 * Copyright (c) 2010-2011 UnixLib Developers
 */

#define _FILE_OFFSET_BITS        64
#define LSEEK_FUNCTION           lseek64
#include "scl_lseek.c"
