/* config.h */

#ifdef IN_GCC

#ifdef HOST_ARM_RISCOS
#include "auto-host-riscos.h"
#else
#include "auto-host.h"
#endif
#include "gansidecl.h"

#ifdef HOST_ARM_RISCOS
#include "config/arm/xm-riscos.h"
#elif HOST_ARM_LINUX
#include "config/arm/xm-linux.h"
#elif HOST_I386_LINUX
#include "config/i386/xm-linux.h"
#elif HOST_I386_FREEBSD
#include "config/i386/xm-freebsd.h"
#elif HOST_MIPS_IRIX
#include "config/mips/xm-iris6.h"
#endif

#include "hwint.h"
#endif
#ifndef HAVE_ATEXIT
#define HAVE_ATEXIT
#endif
#ifndef POSIX
#define POSIX
#endif
