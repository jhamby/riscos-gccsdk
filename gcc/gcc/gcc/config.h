/* config.h */

#ifdef IN_GCC
#include "auto-host.h"
#include "gansidecl.h"

#ifdef HOST_ARM_RISCOS
#include "config/arm/xm-riscos.h"
#elif HOST_ARM_LINUX
#include "config/arm/xm-linux.h"
#elif HOST_I386_LINUX
#include "config/i386/xm-linux.h"
#elif HOST_I386_FREEBSD
#include "config/i386/xm-freebsd.h"
#endif

#include "hwint.h"
#endif
#ifndef HAVE_ATEXIT
#define HAVE_ATEXIT
#endif
#ifndef POSIX
#define POSIX
#endif
