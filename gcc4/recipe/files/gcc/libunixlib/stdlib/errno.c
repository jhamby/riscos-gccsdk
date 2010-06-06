/* Definition of the symbol 'errno'.
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <errno.h>

#if !__UNIXLIB_ERRNO_THREADED
int errno;
#endif

