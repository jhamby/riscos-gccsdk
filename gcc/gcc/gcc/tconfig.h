/* config.h */


#ifdef IN_GCC
/* Provide three core typedefs used by everything, if we are compiling
   GCC.  These used to be found in rtl.h and tree.h, but this is no
   longer practical.  Providing these here rather that system.h allows
   the typedefs to be used everywhere within GCC. */
struct rtx_def;
typedef struct rtx_def *rtx;
struct rtvec_def;
typedef struct rtvec_def *rtvec;
union tree_node;
typedef union tree_node *tree;
#endif

#define GTY(x)
#ifdef IN_GCC
#include "ansidecl.h"
#include "config/arm/xm-riscos.h"
#include "config/arm/riscos-aof.h"
#include "config/arm/arm.h"

#if 0

#ifdef HOST_ARM_RISCOS
#include "config/arm/xm-riscos.h"
#elif HOST_ARM_LINUX
#include "config/arm/xm-linux.h"
#elif HOST_I386_LINUX
#include "config/i386/i386.h"
#include "config/i386/att.h"
#include "config/linux.h"
#include "config/i386/i386afe.h"
#include "config/i386/linux.h"
#elif HOST_I386_FREEBSD
#include "config/i386/xm-freebsd.h"
#elif HOST_MIPS_IRIX
#include "config/mips/xm-iris6.h"
#endif

#endif

#include "defaults.h"
#endif /* IN_GCC */

#ifndef HAVE_ATEXIT
#define HAVE_ATEXIT
#endif
#ifndef POSIX
#define POSIX
#endif

#ifdef GENERATOR_FILE
# undef ENABLE_NLS
#endif

