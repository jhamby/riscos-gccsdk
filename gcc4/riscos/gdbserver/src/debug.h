#ifndef src_debug_h_
#define src_debug_h_

#ifndef NDEBUG
#include <stdio.h>

#define debug printf
#else
#define debug(format, ...) ((void) 0)
#endif

#endif
