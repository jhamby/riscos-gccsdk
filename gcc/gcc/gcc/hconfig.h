/* config.h */

#ifdef IN_GCC
#include "auto-host.h"
#include "gansidecl.h"

#ifdef HOST_ARM_ACORN_RISCOS
#include "config/arm/xm-riscos.h"
#elif HOST_I386_LINUX
#include "config/i386/xm-linux.h"
#endif

#include "hwint.h"
#endif
#ifndef HAVE_ATEXIT
#define HAVE_ATEXIT
#endif
#ifndef POSIX
#define POSIX
#endif
