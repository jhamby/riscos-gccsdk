/* bconfig.h */
#ifndef GCC_BCONFIG_H
#define GCC_BCONFIG_H

#ifdef HOST_ARM_RISCOS
#include "auto-host-riscos.h"
#else
#include "auto-host.h"
#endif

#ifdef IN_GCC
#include "ansidecl.h"
#endif /* IN_GCC */

#endif /* GCC_BCONFIG_H */

