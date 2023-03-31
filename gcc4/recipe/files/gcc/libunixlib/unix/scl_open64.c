/* Unix-style file-descriptor based I/O for the SCL
   Copyright (c) 1997 Peter Burwood
   Copyright (c) 1998-2005 Nick Burrett
   Copyright (c) 2011-2012 UnixLib Developers.  */

#define _FILE_OFFSET_BITS        64
#define OPEN_FUNCTION            open64
#include "scl_open.c"
